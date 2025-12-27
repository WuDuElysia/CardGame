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
        if (!Sprite::initWithFile("res/res/card_general.png")) {
                return false;
        }

	_cardModel = cardModel;
        _isFlipped = _cardModel->isFlipped();
        _backgroundSprite = nullptr;
        _numberSprite = nullptr;
        _suitSprite = nullptr;

        // 设置卡牌初始显示
        this->updateCardDisplay();

        // 设置位置
        this->setPosition(_cardModel->getPosition());

        // 设置卡牌大小
        this->setContentSize(Size(100, 150));

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
        // 移除所有子节点
        this->removeAllChildren();
        
        if (_isFlipped) {
                // 显示正面
                CCLOG("Displaying card front for face: %d, suit: %d", 
                      static_cast<int>(_cardModel->getCardFace()), 
                      static_cast<int>(_cardModel->getCardSuit()));
                
                // 创建数字精灵
                std::string numberRes = CardResConfig::getInstance()->getCardFrontRes(
                        _cardModel->getCardFace(), _cardModel->getCardSuit());
                CCLOG("Number res path: %s", numberRes.c_str());
                _numberSprite = Sprite::create(numberRes);
                if (_numberSprite) {
                        _numberSprite->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
                        _numberSprite->setScale(0.8f);
                        this->addChild(_numberSprite);
                } else {
                        CCLOG("Failed to create number sprite");
                }
                
                // 创建花色精灵
                std::string suitRes = "";
                switch (_cardModel->getCardSuit()) {
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
                                _suitSprite->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 3));
                                _suitSprite->setScale(0.5f);
                                this->addChild(_suitSprite);
                        } else {
                                CCLOG("Failed to create suit sprite");
                        }
                }
        } else {
                // 显示背面
                CCLOG("Displaying card back");
                std::string backRes = CardResConfig::getInstance()->getCardBackRes();
                CCLOG("Back res path: %s", backRes.c_str());
                _backgroundSprite = Sprite::create(backRes);
                if (_backgroundSprite) {
                        _backgroundSprite->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
                        this->addChild(_backgroundSprite);
                } else {
                        CCLOG("Failed to create background sprite");
                }
        }
}



