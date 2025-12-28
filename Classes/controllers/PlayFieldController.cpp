#include "PlayFieldController.h"
#include "managers/PlayFieldManager.h"
#include "views/StackView.h"
#include "managers/CardStackManager.h"
#include "models/UndoModel.h"

PlayFieldController::PlayFieldController(GameModel* gameModel, PlayFieldView* playFieldView, StackView* stackView)
	: _gameModel(gameModel)
	, _playFieldView(playFieldView)
	, _stackView(stackView)
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
	// 初始化主牌区管理器
	_playFieldManager = new PlayFieldManager();
	if (!_playFieldManager->init(_gameModel)) {
		delete _playFieldManager;
		_playFieldManager = nullptr;
		return false;
	}

	// 初始化牌堆管理器
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

				// 设置卡牌位置，加上200的垂直偏移量，让所有卡牌往上移动
				cardView->setPosition(cardModel->getPosition() + cocos2d::Vec2(0, 200));

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

	// 记录撤销操作
	if (_undoManager) {
		UndoModel undoAction;
		undoAction.setActionType("moveCard");
		undoAction.setCardId(cardId);

		// 获取卡牌当前位置
		CardModel* card = _playFieldManager->getCardById(cardId);
		if (card) {
			undoAction.setFromPosition(card->getPosition());
		}

		_undoManager->recordUndoAction(undoAction);
	}

	// 执行卡牌匹配
	bool result = _playFieldManager->matchCard(cardId);

	// 更新视图
	if (_playFieldView) {
		// 移除匹配成功添加到底牌堆的卡牌
		_playFieldView->removeCardViewByCardId(cardId);

		// 更新游戏状态
		if (_gameModel->getGameState() == GameModel::GAME_STATE_WIN ||
			_gameModel->getGameState() == GameModel::GAME_STATE_LOSE) {
			// 游戏结束，执行相应操作
		}
	}

	// 更新底牌显示
	if (_stackView) {
		// 获取最新的底牌
		CardModel* currentTrayCard = _gameModel->getTrayCard();
		if (currentTrayCard) {
			// 创建新的CardView来显示底牌
			auto cardView = CardView::create(currentTrayCard);
			if (cardView) {
				// 设置底牌
				_stackView->setTrayCard(cardView);
			}
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

	// 替换底牌
	bool result = _cardStackManager->replaceTrayCard(card);

	// 如果替换成功，移除主牌区的卡牌
	if (result) {
		_playFieldManager->removeCard(cardId);

		// 更新视图
		if (_playFieldView) {
			_playFieldView->removeCardViewByCardId(cardId);
		}

		// 更新底牌显示
		if (_stackView) {
			// 获取最新的底牌
			CardModel* currentTrayCard = _gameModel->getTrayCard();
			if (currentTrayCard) {
				// 创建新的CardView来显示底牌
				auto cardView = CardView::create(currentTrayCard);
				if (cardView) {
					// 设置底牌
					_stackView->setTrayCard(cardView);
				}
			}
		}
	}

	return result;
}

bool PlayFieldController::replaceTrayWithStackCard() {
	if (!_cardStackManager) {
		return false;
	}

	bool result = _cardStackManager->replaceTrayFromStack();

	// 更新底牌显示
	if (result && _stackView) {
		// 获取最新的底牌
		CardModel* currentTrayCard = _gameModel->getTrayCard();
		if (currentTrayCard) {
			// 创建新的CardView来显示底牌
			auto cardView = CardView::create(currentTrayCard);
			if (cardView) {
				// 设置底牌
				_stackView->setTrayCard(cardView);
			}
		}
	}

	return result;
}

void PlayFieldController::reinitUI() {
	// 先销毁现有的PlayFieldManager和CardStackManager
	if (_playFieldManager) {
		delete _playFieldManager;
		_playFieldManager = nullptr;
	}

	if (_cardStackManager) {
		delete _cardStackManager;
		_cardStackManager = nullptr;
	}

	// 重新初始化
	init();

	// 根据GameModel的最新数据重新创建主牌区卡牌
	if (_playFieldView) {
		// 清空现有的卡牌视图
		_playFieldView->clearAllCardViews();
		
		// 获取GameModel中的最新主牌区卡牌
		const auto& playfieldCards = _gameModel->getPlayfieldCards();
		for (const auto& cardModel : playfieldCards) {
			// 创建卡牌视图
			auto cardView = CardView::create(cardModel.get());
			if (cardView) {
				// 将卡牌视图添加到游戏区域视图中
				_playFieldView->addCardView(cardView);

				// 设置卡牌位置，加上200的垂直偏移量，让所有卡牌往上移动
				cardView->setPosition(cardModel->getPosition() + cocos2d::Vec2(0, 200));

				// 翻转卡牌显示正面
				cardView->setFlipped(true);
			}
		}
	}

	// 更新底牌显示
	if (_stackView) {
		// 获取最新的底牌
		CardModel* currentTrayCard = _gameModel->getTrayCard();
		if (currentTrayCard) {
			// 创建新的CardView来显示底牌
			auto cardView = CardView::create(currentTrayCard);
			if (cardView) {
				// 设置底牌
				_stackView->setTrayCard(cardView);
			}
		}
	}
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




