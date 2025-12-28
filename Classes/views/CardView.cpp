#include "CardView.h"
#include "configs/models/CardResConfig.h"

USING_NS_CC;

CardView* CardView::create(CardModel* cardModel) {
        CardView* pRet = new(std::nothrow) CardView();
        if (pRet && pRet->init(cardModel)) {
                pRet->autorelease();
                return pRet;
        }
        delete pRet;
        return nullptr;
}

bool CardView::init(CardModel* cardModel) {
        // 先初始化一个空精灵，不加载纹理
        if (!Sprite::init()) {
                return false;
        }

	_cardModel = cardModel;
        _isFlipped = _cardModel->isFlipped();
        _backgroundSprite = nullptr;
        _numberSprite = nullptr;
        _suitSprite = nullptr;

        // 设置锚点为左下角，确保位置计算与父视图一致
        this->setAnchorPoint(Vec2(0, 0));
        
        // 设置卡牌大小
        this->setContentSize(Size(100, 150));
        
        // 创建背景精灵，作为卡牌的主体显示
        _backgroundSprite = Sprite::create("res/res/card_general.png");
        if (_backgroundSprite) {
                // 确保背景精灵大小与CardView一致
                _backgroundSprite->setContentSize(this->getContentSize());
                _backgroundSprite->setAnchorPoint(Vec2(0, 0));
                _backgroundSprite->setPosition(Vec2(0, 0));
                this->addChild(_backgroundSprite);
        }
        
        // 设置位置
        this->setPosition(_cardModel->getPosition());
        
        // 设置卡牌初始显示
        this->updateCardDisplay();

        return true;
}

CardModel* CardView::getCardModel() const {
        return _cardModel;
}

void CardView::setCardModel(CardModel* cardModel) {
        _cardModel = cardModel;
        this->updateCardDisplay();
}

void CardView::flipCard() {
        _isFlipped = !_isFlipped;
        this->updateCardDisplay();
}

bool CardView::isFlipped() const {
        return _isFlipped;
}

void CardView::setFlipped(bool flipped) {
        if (_isFlipped == flipped) {
                return;
        }
        
        _isFlipped = flipped;
        this->updateCardDisplay();
}

void CardView::updateCardDisplay() {
        // 移除所有子节点，但保留CardView本身的大小和碰撞箱
        this->removeAllChildren();
        
        // 确保CardView的大小始终是100x150，与显示一致
        this->setContentSize(Size(100, 150));
        
        if (_isFlipped) {
                // 显示正面
                CardFaceType face = _cardModel->getCardFace();
                CardSuitType suit = _cardModel->getCardSuit();
                
                // 验证牌面和花色值是否有效
                bool isValidFace = (face > CardFaceType::CFT_ACE && face < CardFaceType::CFT_MAX) || (face == CardFaceType::CFT_ACE);
                bool isValidSuit = (suit >= CardSuitType::CST_DIAMONDS && suit < CardSuitType::CST_MAX);
                
                CCLOG("Displaying card front for face: %d, suit: %d, valid: %d, %d", 
                      static_cast<int>(face), 
                      static_cast<int>(suit),
                      isValidFace,
                      isValidSuit);
                
                // 创建背景精灵，确保碰撞箱与显示一致
                _backgroundSprite = Sprite::create("res/res/card_general.png");
                if (_backgroundSprite) {
                        // 确保背景精灵大小与CardView一致
                        _backgroundSprite->setContentSize(this->getContentSize());
                        _backgroundSprite->setAnchorPoint(Vec2(0, 0));
                        _backgroundSprite->setPosition(Vec2(0, 0));
                        this->addChild(_backgroundSprite, 0);
                }
                
                if (isValidFace && isValidSuit) {
                        // 创建数字精灵
                        std::string numberRes = CardResConfig::getInstance()->getCardFrontRes(face, suit);
                        CCLOG("Number res path: %s", numberRes.c_str());
                        _numberSprite = Sprite::create(numberRes);
                        if (_numberSprite) {
                                // 数字在中央
                                _numberSprite->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
                                _numberSprite->setScale(0.4f);
                                this->addChild(_numberSprite, 1);
                        }
                        
                        // 创建花色精灵
                        std::string suitRes = "";
                        switch (suit) {
                        case CardSuitType::CST_DIAMONDS:
                                suitRes = "res/res/suits/diamond.png";
                                break;
                        case CardSuitType::CST_HEARTS:
                                suitRes = "res/res/suits/heart.png";
                                break;
                        case CardSuitType::CST_CLUBS:
                                suitRes = "res/res/suits/club.png";
                                break;
                        case CardSuitType::CST_SPADES:
                                suitRes = "res/res/suits/spade.png";
                                break;
                        default:
                                break;
                        }
                        
                        if (!suitRes.empty()) {
                                CCLOG("Suit res path: %s", suitRes.c_str());
                                _suitSprite = Sprite::create(suitRes);
                                if (_suitSprite) {
                                        // 花色在左上角
                                        _suitSprite->setPosition(Vec2(20, this->getContentSize().height - 20));
                                        _suitSprite->setScale(0.5f); // 约20*20大小
                                        this->addChild(_suitSprite, 1);
                                } else {
                                        CCLOG("Failed to create suit sprite");
                                }
                        }
                } else {
                        CCLOG("Invalid card face or suit, displaying fallback");
                        // 使用默认卡牌纹理作为回退
                        _numberSprite = Sprite::create("res/res/card_general.png");
                        if (_numberSprite) {
                                _numberSprite->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
                                _numberSprite->setScale(0.8f);
                                this->addChild(_numberSprite, 1);
                        }
                }
        } else {
                // 显示背面
                CCLOG("Displaying card back");
                std::string backRes = CardResConfig::getInstance()->getCardBackRes();
                CCLOG("Back res path: %s", backRes.c_str());
                _backgroundSprite = Sprite::create(backRes);
                if (_backgroundSprite) {
                        // 确保背景精灵大小与CardView一致，锚点左下角
                        _backgroundSprite->setContentSize(this->getContentSize());
                        _backgroundSprite->setAnchorPoint(Vec2(0, 0));
                        _backgroundSprite->setPosition(Vec2(0, 0));
                        this->addChild(_backgroundSprite, 0);
                } else {
                        CCLOG("Failed to create background sprite, using fallback");
                        // 使用默认卡牌纹理作为回退
                        _backgroundSprite = Sprite::create("res/res/card_general.png");
                        if (_backgroundSprite) {
                                _backgroundSprite->setContentSize(this->getContentSize());
                                _backgroundSprite->setAnchorPoint(Vec2(0, 0));
                                _backgroundSprite->setPosition(Vec2(0, 0));
                                this->addChild(_backgroundSprite, 0);
                        }
                }
        }
}

void CardView::moveCardToPosition(const cocos2d::Vec2& targetPos, float duration, 
                                  const std::function<void()>& callback) {
    // 创建移动动画
    auto moveToAction = cocos2d::MoveTo::create(duration, targetPos);
    
    // 如果提供了回调函数，则在动画完成后执行
    if (callback) {
        auto callFunc = cocos2d::CallFunc::create(callback);
        auto sequence = cocos2d::Sequence::create(moveToAction, callFunc, nullptr);
        this->runAction(sequence);
    } else {
        this->runAction(moveToAction);
    }
}





