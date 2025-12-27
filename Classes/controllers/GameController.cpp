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
	// 鍒涘缓娓告垙妯″瀷
	_gameModel = new GameModel();
	if (!_gameModel->init()) {
		delete _gameModel;
		_gameModel = nullptr;
		return false;
	}

	// 鍒濆鍖栨挙閿€绠＄悊鍣?
	if (!initUndoManager()) {
		return false;
	}

	return true;
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

	bool result = _undoManager->undo();

	// 鏇存柊瑙嗗浘
	if (_gameView && result) {
		// 瑙﹀彂瑙嗗浘鏇存柊
		_gameView->updateGameUI();
	}

	return result;
}

void GameController::setGameView(GameView* gameView) {
	_gameView = gameView;

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
	if (_gameView && _gameView->getPlayFieldView()) {
		_playFieldController = new PlayFieldController(_gameModel, _gameView->getPlayFieldView());
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
			// 浠庣墝鍫嗘彁鍗犱竴寮犲崱鐗?
			// _gameModel->popStackCardToTray();
		}
		
		// 打印加载后的卡牌数量
		CCLOG("After loading level %d: playfield cards count = %d, stack cards count = %d", 
			levelId, 
			(int)_gameModel->getPlayfieldCards().size(), 
			(int)_gameModel->getStackCards().size());
	}

	return true;
}


