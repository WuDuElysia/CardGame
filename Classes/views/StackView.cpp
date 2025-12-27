#include "StackView.h"
#include "configs/models/CardResConfig.h"

USING_NS_CC;


StackView* StackView::create() {
        StackView* pRet = new (std::nothrow) StackView();
        if (pRet && pRet->init()) {
                pRet->autorelease();
                return pRet;
        } else {
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
        _stackSprite = nullptr;
        _countLabel = nullptr;

        // 设置牌堆视图大小为1080*580
        setContentSize(Size(1080, 580));

        // 创建牌堆显示
        std::string cardBackRes = CardResConfig::getInstance()->getCardBackRes();
        CCLOG("Card back res path: %s", cardBackRes.c_str());
        _stackSprite = Sprite::create(cardBackRes);
        if (_stackSprite) {
                _stackSprite->setPosition(_stackPosition);
                addChild(_stackSprite);
        } else {
                CCLOG("Failed to create stack sprite");
        }

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
        if (!cardView) return;

        // 添加视觉堆叠效果，仅用于显示
        auto cardBackSprite = Sprite::create(CardResConfig::getInstance()->getCardBackRes());
        if (cardBackSprite) {
                // 为堆叠效果添加微小偏移
                float offset = _stackCardSprites.size() * 2.0f;
                cardBackSprite->setPosition(_stackPosition + Vec2(offset, -offset));
                cardBackSprite->setZOrder(-_stackCardSprites.size()); // 越晚添加的卡牌越靠后
                addChild(cardBackSprite);
                _stackCardSprites.push_back(cardBackSprite);
        }

        updateStackDisplay();
}

CardView* StackView::removeTopStackCard() {
        if (!_stackCardSprites.empty()) {
                auto topCardBack = _stackCardSprites.back();
                topCardBack->removeFromParent();
                _stackCardSprites.pop_back();

                updateStackDisplay();
        }

        // 实际的CardView由控制器管理，这里只处理视觉效果
        return nullptr; // 实际的CardView由控制器提供
}

void StackView::setTrayCard(CardView* cardView) {
        if (!cardView) return;

        // 移除当前底牌
        if (_currentTrayCard) {
                _currentTrayCard->removeFromParent();
        }

        // 设置新底牌
        _currentTrayCard = cardView;
        cardView->setPosition(_trayPosition);
        // 显示卡牌正面
        cardView->setFlipped(true);
        addChild(cardView);

}

CardView* StackView::getCurrentTrayCard() const {
        return _currentTrayCard;
}

int StackView::getStackCardCount() const {
        return _stackCardSprites.size();
}

void StackView::playDrawCardAnimation() {
        // 播放抽取卡牌的动画效果
        if (_stackSprite) {
                auto scale = ScaleBy::create(0.2f, 1.1f);
                auto scaleBack = scale->reverse();
                _stackSprite->runAction(Sequence::create(scale, scaleBack, nullptr));
        }
}

void StackView::updateStackDisplay() {
        // 更新牌堆数量显示
        int cardCount = _stackCardSprites.size();
        if (_countLabel) {
                _countLabel->setString(StringUtils::format("%d", cardCount));
        }

        // 根据卡牌数量调整显示
        if (cardCount > 0) {
                // 牌堆中有卡牌时，显示牌堆精灵和数量
                if (_stackSprite) {
                        _stackSprite->setVisible(true);
                }
                if (_countLabel) {
                        _countLabel->setVisible(true);
                }
        }
        else {
                // 牌堆为空时，隐藏牌堆精灵但显示数量（0）
                if (_stackSprite) {
                        _stackSprite->setVisible(false);
                }
                if (_countLabel) {
                        _countLabel->setVisible(true);
                }
        }
}

void StackView::setStackPosition(const Vec2& position) {
        _stackPosition = position;
        if (_stackSprite) {
                _stackSprite->setPosition(position);
        }
        if (_countLabel) {
                _countLabel->setPosition(position + Vec2(20, 20));
        }

        // 更新堆叠视觉效果
        for (size_t i = 0; i < _stackCardSprites.size(); ++i) {
                _stackCardSprites[i]->setPosition(position + Vec2(i * 2, i * 2)); // 微小偏移创建堆叠效果
        }
}

void StackView::setTrayPosition(const Vec2& position) {
        _trayPosition = position;

        // 更新当前底牌位置
        if (_currentTrayCard) {
                _currentTrayCard->setPosition(position);
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
        if (_currentTrayCard) {
                auto bounds = _currentTrayCard->getBoundingBox();
                if (bounds.containsPoint(position)) {
                        if (_currentTrayCard->getCardModel() && _trayClickCallback) {
                                _trayClickCallback(_currentTrayCard->getCardModel()->getCardId());
                        }
                        return true;
                }
        }
        return false;
}

bool StackView::onStackClick(const cocos2d::Vec2& position) {
        if (_stackSprite && !_stackCardSprites.empty()) {
                auto bounds = _stackSprite->getBoundingBox();
                if (bounds.containsPoint(position)) {
                        if (_stackClickCallback) {
                                _stackClickCallback();
                        }
                        return true;
                }
        }
        return false;
}