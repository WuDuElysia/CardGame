#include "GameView.h"
#include "configs/models/CardResConfig.h"
#include "models/GameModel.h"

USING_NS_CC;

GameView* GameView::create() {
	GameView* pRet = new (std::nothrow) GameView();
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool GameView::init() {
	if (!Node::init()) {
		return false;
	}

	// 璁剧疆鑳屾櫙棰滆壊
	auto backgroundLayer = cocos2d::LayerColor::create(cocos2d::Color4B(50, 100, 150, 255));
	if (backgroundLayer) {
		// 设置背景层大小为整个屏幕
		auto visibleSize = Director::getInstance()->getVisibleSize();
		backgroundLayer->setContentSize(visibleSize);
		addChild(backgroundLayer, -10); // 涓轰綈浼樼墖锛屾墦鍦ㄥ叏閮ㄦ墖绱犲墠闈?
	}

	// 鍒涘缓涓荤墝鍖鸿鍥?
	_playFieldView = PlayFieldView::create();
	if (_playFieldView) {
		addChild(_playFieldView);
	}

	// 鍒涘缓鐗屽爢瑙嗗浘
	_stackView = StackView::create();
	if (_stackView) {
		addChild(_stackView);
	}

	// 鍒濆鍖朥I
	initUI();

	return true;
}

PlayFieldView* GameView::getPlayFieldView() const {
	return _playFieldView;
}

StackView* GameView::getStackView() const {
	return _stackView;
}

void GameView::initUI() {
	// 设置游戏区域尺寸和位置
	if (_playFieldView) {
		// 主牌区占据整个游戏视图，保持原始尺寸1080*1500
		_playFieldView->setPosition(Vec2(0, 0));
		_playFieldView->setContentSize(Size(1080, 1500)); // 恢复原始尺寸，不修改
		_playFieldView->setLocalZOrder(10); // 确保主牌区显示在牌堆区上方
	}

	// 设置牌堆区尺寸和位置
	if (_stackView) {
		// 堆牌区位于底部，尺寸1080*580
		_stackView->setPosition(Vec2(0, 0));
		_stackView->setContentSize(Size(1080, 580));
		_stackView->setLocalZOrder(5); // 牌堆区位于主牌区下方，避免遮挡
		
		// 设置牌堆和底牌在堆牌区的位置
		_stackView->setStackPosition(Vec2(100, 150));
		_stackView->setTrayPosition(Vec2(300, 150));
	}

	// 鍒涘缓鎾ら攢鎸夐挳
	createUndoButton();

	// 鍒涘缓鍒嗘暟鍜屾鏁版樉绀?
	createScoreAndMoveDisplay();

	// 鍒涘缓娓告垙鐘舵€佹樉绀?
	createGameStateDisplay();
}

void GameView::playEntranceAnimation() {
	// 鎾斁鍏ュ満鍔ㄧ敾
	if (_playFieldView) {
		// 涓荤墝鍖哄叆鍦哄姩鐢?
	}

	if (_stackView) {
		// 鐗屽爢鍏ュ満鍔ㄧ敾
	}

	// 娓告垙UI鍏ュ満鍔ㄧ敾
}

void GameView::updateGameUI() {
	// 鏇存柊娓告垙UI
	if (_playFieldView) {
		_playFieldView->updateStackDisplay();
	}

	if (_stackView) {
		_stackView->updateStackDisplay();
	}
}

void GameView::setUndoButtonClickCallback(const OnUndoButtonClickCallback& callback) {
	_undoButtonClickCallback = callback;
}

void GameView::updateScore(int score) {
	if (_scoreLabel) {
		_scoreLabel->setString(StringUtils::format("Score: %d", score));
	}
}

void GameView::updateMoveCount(int moveCount) {
	if (_moveCountLabel) {
		_moveCountLabel->setString(StringUtils::format("Moves: %d", moveCount));
	}
}

void GameView::setGameState(int gameState) {
	if (_gameStateLabel) {
		std::string stateText;
		switch (gameState) {
		case GameModel::GAME_STATE_PLAYING:
			stateText = "Playing";
			break;
		case GameModel::GAME_STATE_PAUSED:
			stateText = "Paused";
			break;
		case GameModel::GAME_STATE_WIN:
			stateText = "Win!";
			break;
		case GameModel::GAME_STATE_LOSE:
			stateText = "Lose!";
			break;
		default:
			stateText = "Idle";
			break;
		}
		_gameStateLabel->setString(stateText);
	}
}

void GameView::createUndoButton() {
	// 鍒涘缓鎾ら攢鎸夐挳
	auto buttonBg = Sprite::create(CardResConfig::getInstance()->getUndoButtonRes());
	if (!buttonBg) {
		// 濡傛灉娌℃湁鎸夐挳璧勬簮锛屼娇鐢ㄩ粯璁ょ簿鐏?
		buttonBg = Sprite::create();
		if (buttonBg) {
			buttonBg->setContentSize(Size(120, 60));
			buttonBg->setColor(Color3B::GRAY);
		}
	}

	// 鍒涘缓鎾ら攢鎸夐挳鏂囧瓧
	auto buttonLabel = Label::createWithTTF("Undo", CardResConfig::getInstance()->getFontRes(), 20);
	if (buttonLabel) {
		buttonLabel->setPosition(buttonBg->getContentSize() / 2);
		buttonBg->addChild(buttonLabel);
	}

	// 鍒涘缓鑿滃崟椤?
	_undoButton = MenuItemImage::create(buttonBg->getTexture()->getPath(),
		buttonBg->getTexture()->getPath(),
		CC_CALLBACK_1(GameView::onUndoButtonClick, this));

	if (_undoButton) {
		_undoButton->setPosition(Vec2(800, 500));

		// 鍒涘缓鑿滃崟
		auto menu = Menu::create(_undoButton, nullptr);
		menu->setPosition(Vec2::ZERO);
		addChild(menu);
	}
}

void GameView::createScoreAndMoveDisplay() {
	// 鍒涘缓鍒嗘暟鏄剧ず
	_scoreLabel = Label::createWithTTF("Score: 0", CardResConfig::getInstance()->getFontRes(), 24);
	if (_scoreLabel) {
		_scoreLabel->setPosition(Vec2(100, 600));
		_scoreLabel->setColor(Color3B::WHITE);
		addChild(_scoreLabel);
	}

	// 鍒涘缓姝ユ暟鏄剧ず
	_moveCountLabel = Label::createWithTTF("Moves: 0", CardResConfig::getInstance()->getFontRes(), 24);
	if (_moveCountLabel) {
		_moveCountLabel->setPosition(Vec2(300, 600));
		_moveCountLabel->setColor(Color3B::WHITE);
		addChild(_moveCountLabel);
	}
}

void GameView::createGameStateDisplay() {
	// 鍒涘缓娓告垙鐘舵€佹樉绀?
	_gameStateLabel = Label::createWithTTF("Playing", CardResConfig::getInstance()->getFontRes(), 32);
	if (_gameStateLabel) {
		_gameStateLabel->setPosition(Vec2(500, 600));
		_gameStateLabel->setColor(Color3B::GREEN);
		addChild(_gameStateLabel);
	}
}

void GameView::onUndoButtonClick(cocos2d::Ref* sender) {
	// 澶勭悊鎾ら攢鎸夐挳鐐瑰嚮
	if (_undoButtonClickCallback) {
		_undoButtonClickCallback();
	}
}


