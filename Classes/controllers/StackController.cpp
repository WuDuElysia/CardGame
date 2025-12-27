#include "StackController.h"
#include "models/UndoModel.h"

StackController::StackController(GameModel* gameModel, StackView* stackView)
	: _gameModel(gameModel)
	, _stackView(stackView)
	, _undoManager(nullptr)
	, _cardStackManager(nullptr) {
}

StackController::~StackController() {
	if (_cardStackManager) {
		delete _cardStackManager;
		_cardStackManager = nullptr;
	}
}

bool StackController::init() {
	// 鍒濆鍖栫墝鍫嗙鐞嗗櫒
	_cardStackManager = new CardStackManager();
	if (!_cardStackManager->init(_gameModel)) {
		delete _cardStackManager;
		_cardStackManager = nullptr;
		return false;
	}

	// 娉ㄥ唽鐗屽爢鐐瑰嚮鍥炶皟
	if (_stackView) {
		_stackView->setStackClickCallback([this]() {
			this->handleStackClick();
			});

		// 娉ㄥ唽搴曠墝鐐瑰嚮鍥炶皟
		_stackView->setTrayClickCallback([this](int cardId) {
			this->handleTrayClick(cardId);
			});

		// 灏嗘ā鍨嬭〃鐗屽爢鏁版嵁鏇存柊鍒版樉绀轰腑
		const auto& stackCards = _gameModel->getStackCards();
		for (const auto& cardModel : stackCards) {
			auto cardView = CardView::create(cardModel.get());
			if (cardView) {
				cardView->setFlipped(false);

				// 娣诲姞鍒扮墝鍫嗙粯璁ゅ紑寮€
				_stackView->addStackCard(cardView);
			}
		}

		// 灏嗘ā鍨嬭〃搴曠墝鏁版嵁鏇存柊鍒版樉绀轰腑
		CardModel* trayCard = _gameModel->getTrayCard();
		if (trayCard) {
			auto cardView = CardView::create(trayCard);
			if (cardView) {
				// 灏嗘崱鐗囪繘琛岄粯璁ゅ紑寮?
				cardView->setFlipped(true);

				// 璁剧疆搴曠墝
				_stackView->setTrayCard(cardView);
			}
		}
	}

	return true;
}

bool StackController::handleStackClick() {
	// 璁板綍鎾ら攢鎿嶄綔
	if (_undoManager) {
		UndoModel undoAction;
		undoAction.setActionType("replaceTray");

		// 璁板綍褰撳墠搴曠墝淇℃伅
		CardModel* currentTrayCard = _cardStackManager->getCurrentTrayCard();
		if (currentTrayCard) {
			undoAction.setCardId(currentTrayCard->getCardId());
		}

		// 璁板綍褰撳墠鐗屽爢鏁伴噺
		undoAction.setOldStackCardCount(_cardStackManager->getStackCardCount());

		_undoManager->recordUndoAction(undoAction);
	}

	// 浠庡鐢ㄧ墝鍫嗘娊鍙栧崱鐗屾浛鎹㈠簳鐗?
	bool result = replaceTrayWithStackCard();
	
	// 鏇存柊瑙嗗浘
	if (_stackView && result) {
		_stackView->playDrawCardAnimation();
		_stackView->updateStackDisplay();
		
		// 关键修复：创建新的底牌视图并设置到StackView
		CardModel* newTrayCard = _gameModel->getTrayCard();
		if (newTrayCard) {
			// 创建新的底牌视图
			auto newTrayCardView = CardView::create(newTrayCard);
			if (newTrayCardView) {
				// 设置底牌到视图
				_stackView->setTrayCard(newTrayCardView);
			}
		}
	}
	
	return result;
}

bool StackController::handleTrayClick(int cardId) {
	// 搴曠墝鐐瑰嚮澶勭悊
	// 鍙互鏍规嵁闇€瑕佹坊鍔犵浉鍏抽€昏緫
	return true;
}

bool StackController::replaceTrayWithStackCard() {
	if (!_cardStackManager) {
		return false;
	}

	return _cardStackManager->replaceTrayFromStack();
}

bool StackController::isStackEmpty() const {
	if (!_cardStackManager) {
		return true;
	}

	return _cardStackManager->isStackEmpty();
}

void StackController::setUndoManager(UndoManager* undoManager) {
	_undoManager = undoManager;
}


