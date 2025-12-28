#include "PlayFieldView.h"
#include "configs/models/CardResConfig.h"

USING_NS_CC;

PlayFieldView* PlayFieldView::create() {
	PlayFieldView* pRet = new (std::nothrow) PlayFieldView();
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool PlayFieldView::init() {
	if (!Node::init()) {
		return false;
	}

	// 初始化成员变量
	_stackBasePosition = Vec2::ZERO;
	_stackOffset = 20.0f;
	_topCardVisible = true;
	_emptyStateSprite = nullptr;
	_emptyStateLabel = nullptr;

	// 设置锚点为左下角，确保位置计算正确
	setAnchorPoint(Vec2(0, 0));
	// 尺寸由GameView控制，不硬编码
	setContentSize(Size(1080, 920)); // 默认值，会被GameView覆盖

	// 注册触摸事件
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayFieldView::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(PlayFieldView::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	// 注册鼠标事件
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
		bool match = false;
		if ((*it)->getCardModel()) {
			match = ((*it)->getCardModel()->getCardId() == cardId);
		}

		if (match) {
			this->removeChild(*it);
			it = _cardViews.erase(it);
			updateStackDisplay();
			return;
		}
		++it;
	}

	// 如果没找到匹配的CardView，强制更新显示
	CCLOG("Warning: Could not find CardView with cardId %d, updating display anyway", cardId);
	updateStackDisplay();
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

void PlayFieldView::moveCardToTargetWithAnimation(CardView* cardView, const Vec2& targetPos, 
												float duration, const std::function<void()>& callback) {
	if (!cardView) return;

	// 创建移动动画
	auto moveTo = MoveTo::create(duration, targetPos);
	
	// 如果提供了回调函数，则在动画完成后执行
	if (callback) {
		auto callFunc = CallFunc::create([callback]() {
			callback();
		});
		auto sequence = Sequence::create(moveTo, callFunc, nullptr);
		cardView->runAction(sequence);
	} else {
		cardView->runAction(moveTo);
	}
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
	// 更新卡牌显示，使用CardModel中的原始位置
	for (size_t i = 0; i < _cardViews.size(); ++i) {
		auto cardView = _cardViews[i];
		if (!cardView->getCardModel()) continue;

		// 获取CardModel中的原始位置
		Vec2 originalPosition = cardView->getCardModel()->getPosition();

		// 设置卡牌位置为原始位置，加上200的垂直偏移量，让所有卡牌往上移动
		cardView->setPosition(originalPosition + Vec2(0, 200));

		// 根据CardModel中的实际状态设置翻转状态，而不是根据位置
		cardView->setFlipped(cardView->getCardModel()->isFlipped());

		// 确保所有卡牌都可见
		cardView->setVisible(true);
		cardView->setFlipped(true);
	}

	// 更新空状态显示
	setEmptyStateVisible(_cardViews.empty());
}

void PlayFieldView::setTopCardVisible(bool visible) {
	_topCardVisible = visible;
	if (!_cardViews.empty()) {
		_cardViews.back()->setVisible(visible);
	}
}

void PlayFieldView::setEmptyStateVisible(bool visible) {
	// 如果需要显示空状态，但尚未创建空状态显示
	if (visible && !_emptyStateSprite) {
		// 创建空状态精灵
		_emptyStateSprite = Sprite::create(CardResConfig::getInstance()->getEmptyStackRes());
		if (_emptyStateSprite) {
			_emptyStateSprite->setPosition(_stackBasePosition);
			this->addChild(_emptyStateSprite, -1);
		}

		// 创建空状态标签
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

	// 设置空状态可见性
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
	// 将全局坐标转换为PlayFieldView的本地坐标
	Vec2 localPos = this->convertToNodeSpace(pos);

	// 从后往前遍历卡牌，确保返回的是最上层的卡牌
	// 卡牌是按添加顺序存储的，最后添加的在最上层
	for (auto it = _cardViews.rbegin(); it != _cardViews.rend(); ++it) {
		CardView* cardView = *it;
		// 检查CardView是否有效
		if (cardView) {
			auto bounds = cardView->getBoundingBox();
			// 使用本地坐标进行碰撞检测
			if (bounds.containsPoint(localPos)) {
				return cardView;
			}
		}
	}
	return nullptr;
}

void PlayFieldView::clearAllCardViews() {
	// 从父节点移除所有卡牌视图
	for (auto cardView : _cardViews) {
		if (cardView->getParent() == this) {
			cardView->removeFromParent();
		}
	}

	// 清空卡牌视图列表
	_cardViews.clear();
}


