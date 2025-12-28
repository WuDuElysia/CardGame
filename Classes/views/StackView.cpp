#include "StackView.h"
#include "configs/models/CardResConfig.h"

USING_NS_CC;


StackView* StackView::create() {
	StackView* pRet = new (std::nothrow) StackView();
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

bool StackView::init() {
	if (!Node::init()) {
		return false;
	}

	_stackClickCallback = nullptr;
	_trayClickCallback = nullptr;
	_currentTrayCard = nullptr;
	_countLabel = nullptr;

	// 设置锚点为左下角，确保位置计算正确
	setAnchorPoint(Vec2(0, 0));
	// 设置牌堆视图大小为1080*580
	setContentSize(Size(1080, 580));

	// 创建数量显示
	std::string fontRes = CardResConfig::getInstance()->getFontRes();
	CCLOG("Font res path: %s", fontRes.c_str());
	_countLabel = Label::createWithTTF("0", fontRes, 20);
	if (_countLabel) {
		_countLabel->setPosition(Vec2(20, 20));
		addChild(_countLabel);
	}
	else {
		// 对象创建失败，设置"计数器格式为空或外部机制，无数量显示
		CCLOG("Failed to create count label in StackView");
	}

	// 创建触摸事件监听器
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(StackView::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	// 初始化位置
	_stackPosition = Vec2::ZERO;
	_trayPosition = Vec2(100, 0);

	// 初始化显示
	updateStackDisplay();

	return true;
}

void StackView::setStackClickCallback(const std::function<void()>& callback) {
	_stackClickCallback = callback;
}

void StackView::setTrayClickCallback(const std::function<void(int)>& callback) {
	_trayClickCallback = callback;
}

void StackView::addStackCard(CardView* cardView) {
	if (!cardView || !cardView->getCardModel()) return;

	// 设置卡牌为背面朝上
	cardView->setFlipped(false);

	// 使用固定的起始位置来堆叠卡牌
	Vec2 basePosition = Vec2::ZERO;

	// 为堆叠效果添加微小偏移
	float offset = _stackCardViews.size() * 2.0f;
	Vec2 finalPosition = basePosition + Vec2(offset, -offset);

	// 设置卡牌位置，应用stackPosition偏移
	cardView->setPosition(finalPosition + _stackPosition);

	// 设置Z轴顺序，确保正确的堆叠顺序
	cardView->setZOrder(-_stackCardViews.size());

	// 添加到视图中
	addChild(cardView);

	// 保存CardView指针用于后续管理
	_stackCardViews.push_back(cardView);

	updateStackDisplay();
}

CardView* StackView::removeTopStackCard() {
	if (_stackCardViews.empty()) {
		return nullptr;
	}

	// 获取并移除顶部卡牌
	CardView* topCard = _stackCardViews.back();
	_stackCardViews.pop_back();

	// 增加引用计数，确保在返回前不会被释放
	topCard->retain();

	// 不再立即从父节点移除，以便动画能够在原地执行
	// 移除操作将在动画完成后由调用者处理

	updateStackDisplay();

	// 返回实际的CardView对象
	return topCard;
}

void StackView::setTrayCard(CardView* cardView) {
	if (!cardView || !cardView->getCardModel()) return;

	// 移除当前底牌
	if (_currentTrayCard) {
		_currentTrayCard->removeFromParent();
	}

	// 设置新底牌
	_currentTrayCard = cardView;
	// 使用固定的底牌位置，而不是卡牌模型中的原始位置
	// 如果卡牌是从其他位置移动过来的（带有动画），需要调整到正确位置
	cardView->setPosition(_trayPosition);
	// 显示卡牌正面
	cardView->setFlipped(true);
	addChild(cardView);

}

CardView* StackView::getCurrentTrayCard() const {
	return _currentTrayCard;
}

int StackView::getStackCardCount() const {
	return _stackCardViews.size();
}

void StackView::playDrawCardAnimation() {
	// 播放抽取卡牌的动画效果
	// 为顶部卡牌添加动画效果
	if (!_stackCardViews.empty()) {
		CardView* topCard = _stackCardViews.back();
		if (topCard) {
			auto scale = ScaleBy::create(0.2f, 1.1f);
			auto scaleBack = scale->reverse();
			topCard->runAction(Sequence::create(scale, scaleBack, nullptr));
		}
	}
}

void StackView::updateStackDisplay() {
	// 更新牌堆数量显示
	int cardCount = _stackCardViews.size();
	if (_countLabel) {
		_countLabel->setString(StringUtils::format("%d", cardCount));
	}
}

void StackView::clearAllStackCards() {
	// 从父节点移除所有牌堆卡牌
	for (auto cardView : _stackCardViews) {
		if (cardView && cardView->getParent() == this) {
			cardView->removeFromParent();
		}
	}
	
	// 清空卡牌视图列表
	_stackCardViews.clear();
	
	// 更新牌堆显示
	updateStackDisplay();
}

void StackView::setStackPosition(const Vec2& position) {
	_stackPosition = position;
	if (_countLabel) {
		_countLabel->setPosition(position + Vec2(20, 20));
	}
	
	// 更新所有牌堆卡牌位置，使用固定的起始位置，并应用stackPosition偏移
	for (size_t i = 0; i < _stackCardViews.size(); ++i) {
		auto cardView = _stackCardViews[i];
		if (cardView) {
			// 使用固定的起始位置
			Vec2 basePosition = Vec2::ZERO;
			// 为堆叠效果添加微小偏移
			float offset = i * 2.0f;
			Vec2 finalPosition = basePosition + position + Vec2(offset, -offset);
			// 设置卡牌位置
			cardView->setPosition(finalPosition);
		}
	}
}

void StackView::setTrayPosition(const Vec2& position) {
	_trayPosition = position;
	if (_currentTrayCard) {
		// 直接使用trayPosition作为底牌位置，不再使用卡牌模型中的原始位置
		_currentTrayCard->setPosition(position);
	}
}

cocos2d::Vec2 StackView::getTrayPosition() const {
	return _trayPosition;
}

void StackView::moveCardToTargetWithAnimation(CardView* cardView, const Vec2& targetPos, 
												float duration, const std::function<void()>& callback) {
	if (!cardView) return;

	// 创建移动动画
	auto moveTo = MoveTo::create(duration, targetPos);
	
	// 如果提供了回调函数，则在动画完成后执行
	if (callback) {
		auto callFunc = CallFunc::create([this, cardView, callback]() {
			// 动画完成后，不设置位置，因为卡牌可能被移动到其他父节点
			// 位置应该由调用者在回调中设置
			callback();
		});
		auto sequence = Sequence::create(moveTo, callFunc, nullptr);
		cardView->runAction(sequence);
	} else {
		cardView->runAction(moveTo);
	}
}

bool StackView::onTouchBegan(Touch* touch, Event* event) {
	if (!isVisible()) return false;

	auto touchLocation = convertToNodeSpace(touch->getLocation());

	// 先检查底牌点击
	if (onTrayClick(touchLocation)) {
		return true;
	}

	// 再检查牌堆点击
	if (onStackClick(touchLocation)) {
		return true;
	}

	return false;
}

bool StackView::onTrayClick(const cocos2d::Vec2& position) {
	// 提供一个固定的底牌可点击区域，即使没有卡牌
	// 假设卡牌大小为100x140
	auto trayBounds = Rect(_trayPosition.x, _trayPosition.y, 100, 140);
	if (trayBounds.containsPoint(position)) {
		if (_currentTrayCard && _currentTrayCard->getCardModel() && _trayClickCallback) {
			_trayClickCallback(_currentTrayCard->getCardModel()->getCardId());
		}
		return true;
	}
	return false;
}

bool StackView::onStackClick(const cocos2d::Vec2& position) {
	// 提供一个固定的牌堆可点击区域，即使没有卡牌
	// 假设卡牌大小为100x140
	auto stackBounds = Rect(_stackPosition.x, _stackPosition.y, 100, 140);
	if (stackBounds.containsPoint(position)) {
		if (_stackClickCallback) {
			_stackClickCallback();
		}
		return true;
	}
	return false;
}