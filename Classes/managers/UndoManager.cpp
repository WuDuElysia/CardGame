#include "UndoManager.h"

UndoManager::UndoManager() {
    _gameModel = nullptr;
    _maxUndoSteps = 10;
}

UndoManager::~UndoManager() {
    clearUndoHistory();
}

bool UndoManager::init(GameModel* gameModel) {
    if (!gameModel) {
        return false;
    }
    
    _gameModel = gameModel;
    clearUndoHistory();
    return true;
}

void UndoManager::recordUndoAction(const UndoModel& undoAction) {
    if (_undoHistory.size() >= _maxUndoSteps) {
        _undoHistory.erase(_undoHistory.begin());
    }
    
    _undoHistory.push_back(undoAction);
}

bool UndoManager::undo() {
    if (!canUndo() || !_gameModel) {
        return false;
    }
    
    UndoModel& undoAction = _undoHistory.back();
    const std::string& actionType = undoAction.getActionType();
    
    if (actionType == "moveCard") {
        int cardId = undoAction.getCardId();
        CardModel* card = _gameModel->getPlayfieldCardById(cardId);
        if (card) {
            card->setPosition(undoAction.getFromPosition());
        }
    } 
    else if (actionType == "replaceTray") {
        // 鎾ら攢搴曠墝鏇挎崲鎿嶄綔
        // 鑾峰彇褰撳墠搴曠墝
        const auto& trayCards = _gameModel->getTrayCards();
        if (!trayCards.empty()) {
            // 绉婚櫎褰撳墠搴曠墝
            int currentTrayCardId = trayCards.back()->getCardId();
            _gameModel->removeTrayCard(currentTrayCardId);
        }
    }
    
    _undoHistory.pop_back();
    
    return true;
}

bool UndoManager::canUndo() const {
    return !_undoHistory.empty();
}

int UndoManager::getUndoCount() const {
    return _undoHistory.size();
}

void UndoManager::clearUndoHistory() {
    _undoHistory.clear();
}

void UndoManager::setMaxUndoSteps(int maxSteps) {
    _maxUndoSteps = maxSteps;
    
    if (_undoHistory.size() > _maxUndoSteps) {
        _undoHistory.erase(_undoHistory.begin(), _undoHistory.end() - _maxUndoSteps);
    }
}

int UndoManager::getMaxUndoSteps() const {
    return _maxUndoSteps;
}


