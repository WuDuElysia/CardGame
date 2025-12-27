#include "PlayFieldController.h"
#include "managers/PlayFieldManager.h"
#include "managers/CardStackManager.h"
#include "models/UndoModel.h"

PlayFieldController::PlayFieldController(GameModel* gameModel, PlayFieldView* playFieldView)
	: _gameModel(gameModel)
	, _playFieldView(playFieldView)
	, _undoManager(nullptr)
	, _playFieldManager(nullptr)
	, _cardStackManager(nullptr) {
}

PlayFieldController::~PlayFieldController() {
	if (_playFieldManager) {
		delete _playFieldManager;
		_playFieldManager = nullptr;
	}

	if (_cardStackManager) {
		delete _cardStackManager;
		_cardStackManager = nullptr;
	}
}

bool PlayFieldController::init() {
	// 鍒濆鍖栦富鐗屽尯绠＄悊鍣?
	_playFieldManager = new PlayFieldManager();
	if (!_playFieldManager->init(_gameModel)) {
		delete _playFieldManager;
		_playFieldManager = nullptr;
		return false;
	}

	// 鍒濆鍖栫墝鍫嗙鐞嗗櫒
	_cardStackManager = new CardStackManager();
	if (!_cardStackManager->init(_gameModel)) {
		delete _cardStackManager;
		_cardStackManager = nullptr;
		return false;
	}

	// 娉ㄥ唽鍗＄墝鐐瑰嚮鍥炶皟
	if (_playFieldView) {
		_playFieldView->setCardClickCallback([this](int cardId) {
			this->handleCardClick(cardId);
			});
		
		// 将GameModel中的卡牌数据同步到视图中
		const auto& playfieldCards = _gameModel->getPlayfieldCards();
		for (const auto& cardModel : playfieldCards) {
			// 创建卡牌视图
			auto cardView = CardView::create(cardModel.get());
			if (cardView) {
				// 将卡牌视图添加到游戏区域视图中
				_playFieldView->addCardView(cardView);
				
				// 设置卡牌位置
				cardView->setPosition(cardModel->getPosition());
				
				// 翻转卡牌显示正面
				cardView->setFlipped(true);
			}
		}
	}

	return true;
}

bool PlayFieldController::handleCardClick(int cardId) {
	if (!canMoveCard(cardId)) {
		return false;
	}

	// 璁板綍鎾ら攢鎿嶄綔
	if (_undoManager) {
		UndoModel undoAction;
		undoAction.setActionType("moveCard");
		undoAction.setCardId(cardId);

		// 鑾峰彇鍗＄墝褰撳墠浣嶇疆
		CardModel* card = _playFieldManager->getCardById(cardId);
		if (card) {
			undoAction.setFromPosition(card->getPosition());
		}

		_undoManager->recordUndoAction(undoAction);
	}

	// 鎵ц鍗＄墝鍖归厤
	bool result = _playFieldManager->matchCard(cardId);

	// 鏇存柊瑙嗗浘
	if (_playFieldView && result) {
		// 绉婚櫎鍖归厤鎴愬姛鐨勫崱鐗岃鍥?
		_playFieldView->removeCardViewByCardId(cardId);

		// 鏇存柊娓告垙鐘舵€?
		if (_gameModel->getGameState() == GameModel::GAME_STATE_WIN ||
			_gameModel->getGameState() == GameModel::GAME_STATE_LOSE) {
			// 娓告垙缁撴潫锛屾墽琛岀浉搴旀搷浣?
		}
	}
	return result;
}

bool PlayFieldController::replaceTrayWithPlayFieldCard(int cardId) {
	if (!_playFieldManager || !_cardStackManager) {
		return false;
	}

	CardModel* card = _playFieldManager->getCardById(cardId);
	if (!card) {
		return false;
	}

	// 鏇挎崲搴曠墝
	bool result = _cardStackManager->replaceTrayCard(card);

	// 濡傛灉鏇挎崲鎴愬姛锛岀Щ闄や富鐗屽尯鐨勫崱鐗?
	if (result) {
		_playFieldManager->removeCard(cardId);

		// 鏇存柊瑙嗗浘
		if (_playFieldView) {
			_playFieldView->removeCardViewByCardId(cardId);
		}
	}

	return result;
}

bool PlayFieldController::replaceTrayWithStackCard() {
	if (!_cardStackManager) {
		return false;
	}

	return _cardStackManager->replaceTrayFromStack();
}

void PlayFieldController::setUndoManager(UndoManager* undoManager) {
	_undoManager = undoManager;
}

bool PlayFieldController::canMoveCard(int cardId) const {
	if (!_playFieldManager) {
		return false;
	}

	return _playFieldManager->canMatchCard(cardId);
}


