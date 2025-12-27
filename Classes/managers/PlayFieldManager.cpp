#include "PlayFieldManager.h"

PlayFieldManager::PlayFieldManager() {
    _gameModel = nullptr;
}

PlayFieldManager::~PlayFieldManager() {
}

bool PlayFieldManager::init(GameModel* gameModel) {
    if (!gameModel) {
        return false;
    }
    
    _gameModel = gameModel;
    return true;
}

bool PlayFieldManager::canMatchCard(int cardId) const {
    if (!_gameModel) {
        return false;
    }
    
    CardModel* card = _gameModel->getPlayfieldCardById(cardId);
    if (!card) {
        return false;
    }
    
    return _gameModel->canMatchCard(card);
}

bool PlayFieldManager::matchCard(int cardId) {
    if (!_gameModel) {
        return false;
    }
    
    CardModel* card = _gameModel->getPlayfieldCardById(cardId);
    if (!card) {
        return false;
    }
    
    return _gameModel->matchCard(card);
}

CardModel* PlayFieldManager::getCardById(int cardId) const {
    if (!_gameModel) {
        return nullptr;
    }
    
    return _gameModel->getPlayfieldCardById(cardId);
}

void PlayFieldManager::removeCard(int cardId) {
    if (!_gameModel) {
        return;
    }
    
    _gameModel->removePlayfieldCard(cardId);
}

bool PlayFieldManager::addCard(CardModel* card) {
    if (!_gameModel || !card) {
        return false;
    }
    
    _gameModel->addPlayfieldCard(card);
    return true;
}

bool PlayFieldManager::isPlayFieldEmpty() const {
    if (!_gameModel) {
        return true;
    }
    
    return _gameModel->getPlayfieldCards().empty();
}
