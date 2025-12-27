#include "CardStackManager.h"

CardStackManager::CardStackManager() {
    _gameModel = nullptr;
}

CardStackManager::~CardStackManager() {
}

bool CardStackManager::init(GameModel* gameModel) {
    if (!gameModel) {
        return false;
    }
    
    _gameModel = gameModel;
    return true;
}

CardModel* CardStackManager::drawCardFromStack() {
    if (!_gameModel) {
        return nullptr;
    }
    
    return _gameModel->drawStackCard();
}

bool CardStackManager::addCardToStack(CardModel* card) {
    if (!_gameModel || !card) {
        return false;
    }
    
    _gameModel->addStackCard(card);
    return true;
}

bool CardStackManager::isStackEmpty() const {
    if (!_gameModel) {
        return true;
    }
    
    return _gameModel->isStackEmpty();
}

bool CardStackManager::replaceTrayCard(CardModel* card) {
    if (!_gameModel || !card) {
        return false;
    }
    
    _gameModel->addTrayCard(card);
    return true;
}

bool CardStackManager::replaceTrayFromStack() {
    if (!_gameModel) {
        return false;
    }
    
    return _gameModel->replaceTrayWithStackCard();
}

CardModel* CardStackManager::getCurrentTrayCard() const {
    if (!_gameModel) {
        return nullptr;
    }
    
    const auto& trayCards = _gameModel->getTrayCards();
    if (trayCards.empty()) {
        return nullptr;
    }
    
    return trayCards.back().get();
}

int CardStackManager::getStackCardCount() const {
    if (!_gameModel) {
        return 0;
    }
    
    return static_cast<int>(_gameModel->getStackCards().size());
}


