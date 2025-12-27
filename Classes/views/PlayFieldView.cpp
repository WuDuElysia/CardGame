#include "PlayFieldView.h"

USING_NS_CC;

std::shared_ptr<PlayFieldView> PlayFieldView::create() {
        std::shared_ptr<PlayFieldView> pRet = std::make_shared<PlayFieldView>();
        if (pRet && pRet->init()) {
                pRet->autorelease();
                return pRet;
        }
        return nullptr;
}

bool PlayFieldView::init() {
        if (!Node::init()) {
                return false;
        }

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
                _cardViews.push_back(std::shared_ptr<CardView>(cardView));
                this->addChild(cardView);
        }
}

void PlayFieldView::removeCardView(CardView* cardView) {
        auto it = std::find(_cardViews.begin(), _cardViews.end(), std::shared_ptr<CardView>(cardView));
        if (it != _cardViews.end()) {
                _cardViews.erase(it);
                this->removeChild(cardView);
        }
}

std::shared_ptr<CardView> PlayFieldView::getCardViewByCardId(int cardId) const {
        for (auto cardView : _cardViews) {
                if (cardView->getCardModel() && cardView->getCardModel()->getCardId() == cardId) {
                        return cardView;
                }
        }
        return nullptr;
}

void PlayFieldView::setCardClickCallback(const OnCardClickCallback& callback) {
        _onCardClickCallback = callback;
}

bool PlayFieldView::onMouseDown(Event* event) {
        auto mouseEvent = static_cast<EventMouse*>(event);
        auto mouseLocation = mouseEvent->getLocation();
       //
        std::shared_ptr<CardView> clickedCard = findCardAtPosition(mouseLocation);
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

        std::shared_ptr<CardView> clickedCard = findCardAtPosition(touchLocation);
        if (clickedCard && clickedCard->getCardModel() && clickedCard->getCardModel()->isClickable()) {
                return true;
        }
        return false;
}

void PlayFieldView::onTouchEnded(Touch* touch, Event* event) {
        auto touchLocation = touch->getLocation();

        std::shared_ptr<CardView> clickedCard = findCardAtPosition(touchLocation);
        if (clickedCard && clickedCard->getCardModel() && clickedCard->getCardModel()->isClickable()) {
                if (_onCardClickCallback) {
                        _onCardClickCallback(clickedCard->getCardModel()->getCardId());
                }
        }
}

std::shared_ptr<CardView> PlayFieldView::findCardAtPosition(const Vec2& pos) const {
        // 从后往前检查，确保上层卡牌优先被检测
        for (auto it = _cardViews.rbegin(); it != _cardViews.rend(); ++it) {
                std::shared_ptr<CardView> cardView = *it;
                auto bounds = cardView->getBoundingBox();
                if (bounds.containsPoint(this->convertToNodeSpace(pos))) {
                        return cardView;
                }
        }
        return nullptr;
}