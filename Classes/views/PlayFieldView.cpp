#include "PlayFieldView.h"
#include "configs/models/CardResConfig.h"

USING_NS_CC;

PlayFieldView* PlayFieldView::create() {
	PlayFieldView* pRet = new (std::nothrow) PlayFieldView();
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool PlayFieldView::init() {
	if (!Node::init()) {
		return false;
	}

	// 鍒濆鍖栨垚鍛樺彉閲?
	_stackBasePosition = Vec2::ZERO;
	_stackOffset = 20.0f;
	_topCardVisible = true;
	_emptyStateSprite = nullptr;
	_emptyStateLabel = nullptr;

	// 设置主牌区尺寸为1080*1500
	setContentSize(Size(1080, 1500));

	// 娉ㄥ唽瑙︽懜浜嬩欢
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayFieldView::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(PlayFieldView::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	// 娉ㄥ唽榧犳爣浜嬩欢
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(PlayFieldView::onMouseDown, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);

	return true;
}

void PlayFieldView::addCardView(CardView* cardView) {
	if (cardView) {
		_cardViews.push_back(cardView);
		this->addChild(cardView);
		// 不调用updateStackDisplay，保持卡牌原位置
		// updateStackDisplay();
	}
}

void PlayFieldView::removeCardView(CardView* cardView) {
	auto it = std::find(_cardViews.begin(), _cardViews.end(), cardView);
	if (it != _cardViews.end()) {
		_cardViews.erase(it);
		this->removeChild(cardView);
		updateStackDisplay();
	}
}

void PlayFieldView::removeCardViewByCardId(int cardId) {
	auto it = _cardViews.begin();
	while (it != _cardViews.end()) {
		if ((*it)->getCardModel() && (*it)->getCardModel()->getCardId() == cardId) {
			this->removeChild(*it);
			it = _cardViews.erase(it);
			updateStackDisplay();
			return;
		}
		++it;
	}
}

CardView* PlayFieldView::getCardViewByCardId(int cardId) const {
	for (auto cardView : _cardViews) {
		if (cardView->getCardModel() && cardView->getCardModel()->getCardId() == cardId) {
			return cardView;
		}
	}
	return nullptr;
}

void PlayFieldView::moveCardToPosition(CardView* cardView, const Vec2& position, float duration) {
	if (!cardView) return;

	auto moveTo = MoveTo::create(duration, position);
	cardView->runAction(moveTo);
}

void PlayFieldView::flipCard(CardView* cardView, float duration) {
	if (!cardView) return;

	// 缂╂斁鍔ㄧ敾瀹炵幇缈昏浆鍔ㄧ敾鏁堟灉
	auto scaleX = ScaleTo::create(duration / 2, 0.1f, 1.0f);
	auto scaleBack = ScaleTo::create(duration / 2, 1.0f, 1.0f);

	// 缈昏浆鍓嶅悗鍒囨崲鍗＄墝姝ｅ弽闈?
	auto flipCallback = CallFunc::create([cardView]() {
		cardView->flipCard();
		});

	auto sequence = Sequence::create(scaleX, flipCallback, scaleBack, nullptr);
	cardView->runAction(sequence);
}

void PlayFieldView::scaleCard(CardView* cardView, const Vec2& scale, float duration) {
	if (!cardView) return;

	auto scaleTo = ScaleTo::create(duration, scale.x, scale.y);
	cardView->runAction(scaleTo);
}

void PlayFieldView::highlightCard(CardView* cardView, bool highlight, float duration) {
	if (!cardView) return;

	Vec2 targetScale = highlight ? Vec2(1.1f, 1.1f) : Vec2(1.0f, 1.0f);
	auto scaleTo = ScaleTo::create(duration, targetScale.x, targetScale.y);
	cardView->runAction(scaleTo);
}

void PlayFieldView::setCardStackPosition(const Vec2& basePosition, float offset) {
	_stackBasePosition = basePosition;
	_stackOffset = offset;
	updateStackDisplay();
}

void PlayFieldView::updateStackDisplay() {
	// 鏇存柊鍗＄墝鍫嗗彔鏄剧ず
	for (size_t i = 0; i < _cardViews.size(); ++i) {
		auto cardView = _cardViews[i];
		Vec2 position = _stackBasePosition + Vec2(0, i * _stackOffset);

		// 鍙湁鏈€椤跺眰鍗＄墝鍙锛屽叾浠栧崱鐗岄儴鍒嗗彲瑙?
		if (i == _cardViews.size() - 1) {
			cardView->setVisible(_topCardVisible);
		}
		else {
			cardView->setVisible(true);
		}

		// 鏇存柊浣嶇疆
		cardView->setPosition(position);
	}

	// 鏇存柊绌虹姸鎬佹樉绀?
	setEmptyStateVisible(_cardViews.empty());
}

void PlayFieldView::setTopCardVisible(bool visible) {
	_topCardVisible = visible;
	if (!_cardViews.empty()) {
		_cardViews.back()->setVisible(visible);
	}
}

void PlayFieldView::setEmptyStateVisible(bool visible) {
	// 濡傛灉闇€瑕佹樉绀虹┖鐘舵€侊紝浣嗗皻鏈垱寤虹┖鐘舵€佹樉绀?
	if (visible && !_emptyStateSprite) {
		// 鍒涘缓绌虹姸鎬佺簿鐏?
		_emptyStateSprite = Sprite::create(CardResConfig::getInstance()->getEmptyStackRes());
		if (_emptyStateSprite) {
			_emptyStateSprite->setPosition(_stackBasePosition);
			this->addChild(_emptyStateSprite, -1);
		}

		// 鍒涘缓绌虹姸鎬佹爣绛?
		TTFConfig ttfConfig;
		ttfConfig.fontFilePath = CardResConfig::getInstance()->getFontRes();
		ttfConfig.fontSize = 24;
		ttfConfig.glyphs = GlyphCollection::DYNAMIC;
		ttfConfig.outlineSize = 0;
		ttfConfig.customGlyphs = nullptr;
		ttfConfig.distanceFieldEnabled = false;

		_emptyStateLabel = Label::createWithTTF(ttfConfig, "nullcard");
		if (_emptyStateLabel) {
			_emptyStateLabel->setPosition(_stackBasePosition + Vec2(0, -50));
			_emptyStateLabel->setColor(Color3B::GRAY);
			this->addChild(_emptyStateLabel, -1);
		}
	}

	// 璁剧疆绌虹姸鎬佸彲瑙佹€?
	if (_emptyStateSprite) {
		_emptyStateSprite->setVisible(visible);
	}
	if (_emptyStateLabel) {
		_emptyStateLabel->setVisible(visible);
	}
}

void PlayFieldView::setCardClickCallback(const OnCardClickCallback& callback) {
	_onCardClickCallback = callback;
}

bool PlayFieldView::onMouseDown(Event* event) {
	auto mouseEvent = static_cast<EventMouse*>(event);
	auto mouseLocation = mouseEvent->getLocation();

	CardView* clickedCard = findCardAtPosition(mouseLocation);
	if (clickedCard && clickedCard->getCardModel() && clickedCard->getCardModel()->isClickable()) {
		if (_onCardClickCallback) {
			_onCardClickCallback(clickedCard->getCardModel()->getCardId());
		}
		return true;
	}
	return false;
}

bool PlayFieldView::onTouchBegan(Touch* touch, Event* event) {
	auto touchLocation = touch->getLocation();

	CardView* clickedCard = findCardAtPosition(touchLocation);
	if (clickedCard && clickedCard->getCardModel() && clickedCard->getCardModel()->isClickable()) {
		return true;
	}
	return false;
}

void PlayFieldView::onTouchEnded(Touch* touch, Event* event) {
	auto touchLocation = touch->getLocation();

	CardView* clickedCard = findCardAtPosition(touchLocation);
	if (clickedCard && clickedCard->getCardModel() && clickedCard->getCardModel()->isClickable()) {
		if (_onCardClickCallback) {
			_onCardClickCallback(clickedCard->getCardModel()->getCardId());
		}
	}
}

CardView* PlayFieldView::findCardAtPosition(const Vec2& pos) const {
	// 浠庡悗寰€鍓嶉亶鍘嗭紝纭繚鐐瑰嚮妫€娴嬬殑姝ｇ‘鎬?
	for (auto it = _cardViews.rbegin(); it != _cardViews.rend(); ++it) {
		CardView* cardView = *it;
		auto bounds = cardView->getBoundingBox();
		// 由于游戏区域视图位置是(0,0)，不需要转换触摸位置
		if (bounds.containsPoint(pos)) {
			return cardView;
		}
	}
	return nullptr;
}

