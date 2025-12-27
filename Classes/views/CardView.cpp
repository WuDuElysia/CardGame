#include "CardView.h"
#include "configs/models/CardResConfig.h"

USING_NS_CC;

std::shared_ptr<CardView> CardView::create(CardModel* cardModel) {
        std::shared_ptr<CardView> pRet = std::make_shared<CardView>();
        if (pRet && pRet->init(cardModel)) {
                pRet->autorelease();
                return pRet;
        }
        return nullptr;
}

bool CardView::init(CardModel* cardModel) {
        if (!Sprite::init()) {
                return false;
        }

	_cardModel = std::shared_ptr<CardModel>(cardModel);
        _isFlipped = false;

        // 设置卡牌初始显示
        std::string backRes = CardResConfig::getInstance()->getCardBackRes();
        this->setTexture(backRes);

        // 设置位置
        this->setPosition(_cardModel->getPosition());

        return true;
}

CardModel* CardView::getCardModel() const {
        return _cardModel.get();
}

void CardView::setCardModel(CardModel* cardModel) {
        _cardModel = std::shared_ptr<CardModel>(cardModel);
}

void CardView::flipCard() {
        _isFlipped = !_isFlipped;

        if (_isFlipped) {
                // 显示正面
                std::string frontRes = CardResConfig::getInstance()->getCardFrontRes(
                        _cardModel->getCardFace(), _cardModel->getCardSuit());
                this->setTexture(frontRes);
        }
        else {
                // 显示背面
                std::string backRes = CardResConfig::getInstance()->getCardBackRes();
                this->setTexture(backRes);
        }
}

bool CardView::isFlipped() const {
        return _isFlipped;
}

