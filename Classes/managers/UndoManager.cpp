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

		// 从底牌堆中找到要撤销的卡牌
		CardModel* card = _gameModel->getTrayCardById(cardId);
		if (card) {
			// 使用安全的方法将卡牌从底牌堆移动到主牌区
			bool result = _gameModel->moveTrayCardToPlayfield(cardId);

			if (result) {
				// 设置回原来的位置
				card->setPosition(undoAction.getFromPosition());
			}
		}
	}
	else if (actionType == "replaceTray") {
		// 撤销底牌替换操作
		// 获取当前底牌
		const auto& trayCards = _gameModel->getTrayCards();
		if (!trayCards.empty()) {
			// 从底牌堆移除当前底牌（栈顶元素）
			int currentTrayCardId = trayCards.back()->getCardId();
			CardModel* currentTrayCard = _gameModel->getTrayCardById(currentTrayCardId);
			if (currentTrayCard) {
				// 从底牌堆移除当前底牌
				_gameModel->removeTrayCard(currentTrayCardId);

				// 将当前底牌添加回牌堆
				_gameModel->addStackCard(currentTrayCard);
			}
		}

		// 如果之前有底牌，恢复它
		int oldCardId = undoAction.getCardId();
		if (oldCardId != -1) {
			// 这是之前的底牌，应该恢复到底牌堆
			// 从牌堆中找到这张牌并移回底牌堆
			CardModel* oldTrayCard = _gameModel->getStackCardById(oldCardId);
			if (oldTrayCard) {
				// 将牌从牌堆移除
				_gameModel->removeStackCard(oldCardId);
				// 将牌添加到底牌堆（作为新的底牌）
				_gameModel->addTrayCard(oldTrayCard);
			}
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

UndoModel& UndoManager::getLastUndoAction() {
	return _undoHistory.back();
}

int UndoManager::getMaxUndoSteps() const {
	return _maxUndoSteps;
}



