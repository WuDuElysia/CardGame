#include "GameController.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "services/CardMatchingService.h"

GameController::GameController() {
	_gameModel = nullptr;
	_gameView = nullptr;
	_playFieldController = nullptr;
	_stackController = nullptr;
	_undoManager = nullptr;
	_currentLevelId = 0;
	_score = 0;
	_moveCount = 0;
}

GameController::~GameController() {
	if (_playFieldController) {
		delete _playFieldController;
		_playFieldController = nullptr;
	}

	if (_stackController) {
		delete _stackController;
		_stackController = nullptr;
	}

	if (_undoManager) {
		delete _undoManager;
		_undoManager = nullptr;
	}

	if (_gameModel) {
		delete _gameModel;
		_gameModel = nullptr;
	}
}

bool GameController::init() {
	// 创建游戏模型
	_gameModel = new GameModel();
	if (!_gameModel->init()) {
		delete _gameModel;
		_gameModel = nullptr;
		return false;
	}

	// 初始化撤销管理器
	if (!initUndoManager()) {
		return false;
	}

	return true;
}

void GameController::moveCardAnimation(CardView* cardView, cocos2d::Node* fromView, cocos2d::Node* toView,
	const cocos2d::Vec2& targetPos, float duration,
	const std::function<void()>& callback) {
	if (!cardView) return;

	// 从起始视图中移除卡牌
	if (cardView->getParent() == fromView) {
		cardView->retain(); // 保留引用，防止被删除
		fromView->removeChild(cardView);
	}

	// 将卡牌添加到场景中，确保动画期间不会被删除
	cocos2d::Node* scene = fromView->getParent();
	if (scene) {
		scene->addChild(cardView);
	}

	// 执行移动动画
	auto moveTo = cocos2d::MoveTo::create(duration, targetPos);

	// 如果提供了回调函数，则在动画完成后执行
	if (callback) {
		auto callFunc = cocos2d::CallFunc::create([this, cardView, toView, callback]() {
			// 动画完成后，将卡牌添加到目标视图
			if (cardView && toView) {
				// 将卡牌添加到目标视图
				toView->addChild(cardView);
				cardView->release(); // 释放之前保留的引用
				callback();
			}
			});
		auto sequence = cocos2d::Sequence::create(moveTo, callFunc, nullptr);
		cardView->runAction(sequence);
	}
	else {
		cardView->runAction(moveTo);
	}
}

void GameController::moveCardUndoAnimation(CardView* cardView, const cocos2d::Vec2& fromPos,
	const cocos2d::Vec2& toPos, float duration,
	const std::function<void()>& callback) {
	if (!cardView) return;

	// 设置卡牌的起始位置
	cardView->setPosition(fromPos);

	// 执行移动动画
	auto moveTo = cocos2d::MoveTo::create(duration, toPos);

	// 如果提供了回调函数，则在动画完成后执行
	if (callback) {
		auto callFunc = cocos2d::CallFunc::create([callback]() {
			callback();
			});
		auto sequence = cocos2d::Sequence::create(moveTo, callFunc, nullptr);
		cardView->runAction(sequence);
	}
	else {
		cardView->runAction(moveTo);
	}
}

bool GameController::startGame(int levelId) {
	CCLOG("Starting game with level ID: %d", levelId);
	_currentLevelId = levelId;

	// 鍔犺浇鍏冲崱閰嶇疆
	if (!loadLevel(levelId)) {
		return false;
	}

	// 打印当前牌堆里的卡牌数量
	if (_gameModel) {
		CCLOG("Game started: playfield cards count = %d, stack cards count = %d",
			(int)_gameModel->getPlayfieldCards().size(),
			(int)_gameModel->getStackCards().size());
	}

	// 鍒濆鍖栧瓙鎺у埗鍣?
	if (!initControllers()) {
		return false;
	}

	// 鍒濆鍖栨父鎴忚鍥?
	if (_gameView) {
		_gameView->initUI();
		_gameView->playEntranceAnimation();
	}

	// 璁剧疆娓告垙鐘舵€?
	if (_gameModel) {
		_gameModel->setGameState(GameModel::GAME_STATE_PLAYING);
		CCLOG("Game state set to: PLAYING");
	}

	return true;
}

void GameController::pauseGame() {
	if (_gameModel) {
		_gameModel->setGameState(GameModel::GAME_STATE_PAUSED);
	}
}

void GameController::resumeGame() {
	if (_gameModel) {
		_gameModel->setGameState(GameModel::GAME_STATE_PLAYING);
	}
}

void GameController::endGame() {
	if (_gameModel) {
		_gameModel->setGameState(GameModel::GAME_STATE_IDLE);
	}
}

bool GameController::undo() {
	if (!_undoManager || !_undoManager->canUndo()) {
		return false;
	}

	// 获取撤销操作的详细信息
	UndoModel lastAction = _undoManager->getLastUndoAction(); // 复制而不是引用，避免在执行undo后引用失效

	// 在执行撤销前，先获取需要动画的卡牌视图
	CardView* trayCardView = nullptr;
	int cardId = -1;
	cocos2d::Vec2 originalPosition = cocos2d::Vec2(0, 0);
	bool needsAnimation = false;

	if (lastAction.getActionType() == "moveCard") {
		cardId = lastAction.getCardId();
		originalPosition = lastAction.getFromPosition();

		// 获取当前在底牌区的卡牌视图（在执行撤销操作前）
		if (_stackController && _playFieldController && _gameView) {
			StackView* stackView = _gameView->getStackView();
			if (stackView) {
				trayCardView = stackView->getCurrentTrayCard();
				needsAnimation = (trayCardView && trayCardView->getCardModel() && trayCardView->getCardModel()->getCardId() == cardId);
			}
		}
	}

	// 执行撤销操作
	bool result = _undoManager->undo();

	// 重新初始化UI，根据GameModel的最新数据
	if (result) {
		// 根据撤销操作类型执行相应的动画
		if (lastAction.getActionType() == "moveCard" && needsAnimation && trayCardView) {
			// 这是主牌区到备用牌区的撤销操作，需要动画
			// 获取当前StackView
			if (_stackController && _playFieldController && _gameView) {
				StackView* stackView = _gameView->getStackView();
				PlayFieldView* playFieldView = _gameView->getPlayFieldView();

				if (stackView && playFieldView) {
					// 获取底牌的当前位置（全局坐标）
					cocos2d::Vec2 currentTrayPosition = stackView->convertToWorldSpace(trayCardView->getPosition());
					// 获取原始位置（全局坐标）
					cocos2d::Vec2 targetOriginalPosition = playFieldView->convertToWorldSpace(originalPosition);

					// 执行反向移动动画：从底牌位置移动到原位置
					moveCardUndoAnimation(trayCardView, currentTrayPosition, targetOriginalPosition, 0.5f, [this, cardId]() {
						// 动画完成后，不要直接操作原来的trayCardView
						// 因为在执行undo()时，卡牌模型已经从底牌堆移动到主牌区
						// 而reinitUI()会重新创建所有卡牌视图
						
						// 直接重新初始化UI以确保状态一致
						if (_playFieldController) {
							_playFieldController->reinitUI();
						}
						if (_stackController) {
							_stackController->reinitUI();
						}

						// 更新游戏UI
						if (_gameView) {
							_gameView->updateGameUI();
						}
						});
					return result; // 已经处理了动画，直接返回
				}
			}
		}
		else if (lastAction.getActionType() == "replaceTray") {
			// 这是备用牌区到备用牌区的撤销操作，需要动画
			// 获取当前StackView
			if (_stackController && _gameView) {
				StackView* stackView = _gameView->getStackView();
				if (stackView) {
					// 获取当前底牌（即将被移除的牌）
					CardView* currentTrayCardView = stackView->getCurrentTrayCard();
					if (currentTrayCardView) {
						// 从StackView中移除当前底牌，准备动画
						if (currentTrayCardView->getParent() == stackView) {
							currentTrayCardView->retain(); // 保留引用
							stackView->removeChild(currentTrayCardView); // 从StackView中移除底牌
						}

						// 获取备用牌堆顶部位置（相对于StackView的坐标）
						// 计算备用牌堆顶部的准确位置，基于当前牌堆大小
						cocos2d::Vec2 stackTopPosition = cocos2d::Vec2(stackView->getStackCardCount() * 2.0f, -stackView->getStackCardCount() * 2.0f);

						// 执行移动动画：从底牌位置移动到备用牌堆位置
						stackView->moveCardToTargetWithAnimation(currentTrayCardView, stackTopPosition, 0.5f, [this, currentTrayCardView, stackView]() {
							// 动画完成后，将卡牌添加到备用牌堆视图
							if (currentTrayCardView && stackView) {
								// 添加回备用牌堆
								stackView->addStackCard(currentTrayCardView);
								currentTrayCardView->release(); // 释放引用

								// 重新初始化UI以确保状态一致
								if (_stackController) {
									_stackController->reinitUI();
								}

								// 更新游戏UI
								if (_gameView) {
									_gameView->updateGameUI();
								}
							}
							});
						return result; // 已经处理了动画，直接返回
					}
				}
			}
		}

		// 如果没有特殊动画处理，或者动画不适用，则直接重新初始化UI
		// 重新初始化主牌区UI
		if (_playFieldController) {
			_playFieldController->reinitUI();
		}

		// 重新初始化牌堆区UI
		if (_stackController) {
			_stackController->reinitUI();
		}

		// 更新游戏UI
		if (_gameView) {
			_gameView->updateGameUI();
		}
	}

	return result;
}

void GameController::setGameView(GameView* gameView) {
	_gameView = gameView;

	// 设置撤销按钮回调
	if (_gameView) {
		_gameView->setUndoButtonClickCallback([this]() {
			this->undo();
			});
	}

	// 濡傛灉瀛愭帶鍒跺櫒宸茬粡鍒濆鍖栵紝璁剧疆鍏惰鍥?
	if (_playFieldController && _gameView->getPlayFieldView()) {
		// 涓荤墝鍖鸿鍥惧凡缁忓湪GameView涓垱寤?
	}

	if (_stackController && _gameView->getStackView()) {
		// 鐗屽爢瑙嗗浘宸茬粡鍦℅ameView涓垱寤?
	}
}

GameModel* GameController::getGameModel() const {
	return _gameModel;
}

PlayFieldController* GameController::getPlayFieldController() const {
	return _playFieldController;
}

StackController* GameController::getStackController() const {
	return _stackController;
}

UndoManager* GameController::getUndoManager() const {
	return _undoManager;
}

bool GameController::initControllers() {
	// 鍒濆鍖栦富鐗屽尯鎺у埗鍣?
	if (_gameView && _gameView->getPlayFieldView() && _gameView->getStackView()) {
		_playFieldController = new PlayFieldController(_gameModel, _gameView->getPlayFieldView(), _gameView->getStackView());
		if (!_playFieldController->init()) {
			delete _playFieldController;
			_playFieldController = nullptr;
			return false;
		}

		// 璁剧疆鎾ら攢绠＄悊鍣?
		_playFieldController->setUndoManager(_undoManager);
	}

	// 鍒濆鍖栫墝鍫嗘帶鍒跺櫒
	if (_gameView && _gameView->getStackView()) {
		_stackController = new StackController(_gameModel, _gameView->getStackView());
		if (!_stackController->init()) {
			delete _stackController;
			_stackController = nullptr;
			return false;
		}

		// 璁剧疆鎾ら攢绠＄悊鍣?
		_stackController->setUndoManager(_undoManager);
	}

	return true;
}

bool GameController::initUndoManager() {
	_undoManager = new UndoManager();
	if (!_undoManager->init(_gameModel)) {
		delete _undoManager;
		_undoManager = nullptr;
		return false;
	}

	return true;
}

bool GameController::loadLevel(int levelId) {
	// 鍔犺浇鍏冲崱閰嶇疆
	auto levelConfig = LevelConfigLoader::loadLevelConfig(levelId);
	if (!levelConfig) {
		CCLOG("Failed to load level config for level: %d", levelId);
		return false;
	}

	// 鏍规嵁閰嶇疆鍒濆鍖栨父鎴忔ā鍨嬬殑鍗＄墝
	if (_gameModel) {
		// 闃叉鍏冲崱鏁版嵁涓己鏈夊崱鐗?
		// _gameModel->clearAllCards();

		static int cardIdCounter = 1; // 用于生成唯一卡牌ID

		// 打印加载前的卡牌数量
		CCLOG("Before loading level %d: playfield cards count = %d, stack cards count = %d",
			levelId,
			(int)_gameModel->getPlayfieldCards().size(),
			(int)_gameModel->getStackCards().size());

		// 娣诲姞涓荤墝鍖虹墝鐗?
		const auto& playfieldCards = levelConfig->getPlayfieldCards();
		CCLOG("Loading %d playfield cards from level config", (int)playfieldCards.size());
		for (const auto& cardConfig : playfieldCards) {
			// 使用带参数的构造函数创建卡牌模型，参数顺序：cardId, face, suit, position
			CardModel* card = new CardModel(cardIdCounter++, cardConfig.cardFace, cardConfig.cardSuit, cardConfig.position);
			card->setClickable(true);
			_gameModel->addPlayfieldCard(card);
		}

		// 娣诲姞鐗屽爢鍗＄墝
		const auto& stackCards = levelConfig->getStackCards();
		CCLOG("Loading %d stack cards from level config", (int)stackCards.size());
		for (const auto& cardConfig : stackCards) {
			// 使用带参数的构造函数创建卡牌模型，参数顺序：cardId, face, suit, position
			CardModel* card = new CardModel(cardIdCounter++, cardConfig.cardFace, cardConfig.cardSuit, cardConfig.position);
			_gameModel->addStackCard(card);
		}

		// 璁剧疆绗竴寮犲崱鐗?
		if (_gameModel->getStackCards().size() > 0) {
			// 浠庣墝鍫嗘彁鍗犱竴寮犲崱鐗?浣滀负鍒濇搴曠墝
			_gameModel->replaceTrayWithStackCard();
		}

		// 打印加载后的卡牌数量
		CCLOG("After loading level %d: playfield cards count = %d, stack cards count = %d",
			levelId,
			(int)_gameModel->getPlayfieldCards().size(),
			(int)_gameModel->getStackCards().size());
	}

	return true;
}



