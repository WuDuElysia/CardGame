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

	// 设置锚点为左下角，确保整个坐标系统一致
	setAnchorPoint(Vec2(0, 0));
	
	// 设置背景颜色
	auto backgroundLayer = cocos2d::LayerColor::create(cocos2d::Color4B(50, 100, 150, 255));
	if (backgroundLayer) {
		// 设置背景层大小为整个屏幕
		auto visibleSize = Director::getInstance()->getVisibleSize();
		backgroundLayer->setContentSize(Size(1080, 2080)); // 总尺寸1080*2080
		backgroundLayer->setAnchorPoint(Vec2(0, 0));
		addChild(backgroundLayer, -10); // 为齐优片，打在全部片前面
	}

	// 创建主牌区视图
	_playFieldView = PlayFieldView::create();
	if (_playFieldView) {
		addChild(_playFieldView);
	}

	// 创建牌堆视图
	_stackView = StackView::create();
	if (_stackView) {
		addChild(_stackView);
	}

	// 初始化UI
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
	// cocos2dx坐标系：原点在左下角，Y轴向上
	// 主牌区在上半部分，备用牌区在下半部分，总尺寸1080*2080
	
	// 设置游戏区域尺寸和位置
	if (_playFieldView) {
		// 主牌区位于上方，底部与备用牌区顶部对齐
		_playFieldView->setPosition(Vec2(0, 580)); // Y=580，位于备用牌区上方
		_playFieldView->setContentSize(Size(1080, 1500)); // 主牌区尺寸1080*1500
		_playFieldView->setLocalZOrder(10); // 确保主牌区显示在牌堆区上方
		// 主牌区卡牌位置由CardModel中的position决定（相对于主牌区的本地坐标）
		_playFieldView->updateStackDisplay(); // 初始化主牌区卡牌位置
	}

	// 设置牌堆区尺寸和位置
	if (_stackView) {
		// 备用牌区位于底部，原点对齐
		_stackView->setPosition(Vec2(0, 0)); // Y=0，位于最底部
		_stackView->setContentSize(Size(1080, 580)); // 备用牌区尺寸1080*580
		_stackView->setLocalZOrder(5); // 牌堆区位于主牌区下方
		
		// 初始化牌堆和底牌位置，将它们设置在可视区域内
		// cocos2d-x坐标系：Y轴向上，Y=0是底部，Y=580是StackView顶部
		_stackView->setStackPosition(Vec2(100, 100)); // 将牌堆位置调整到可视区域内
		_stackView->setTrayPosition(Vec2(300, 100)); // 将底牌位置调整到可视区域内
	}

	// 创建撤销按钮
	createUndoButton();
}

void GameView::playEntranceAnimation() {
	// 播放入场动画
	if (_playFieldView) {
		// 主牌区入场动画
	}

	if (_stackView) {
		// 牌堆入场动画
	}

	// 游戏UI入场动画
}

void GameView::updateGameUI() {
	// 更新游戏UI
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
	// 创建撤销按钮
	auto buttonBg = Sprite::create(CardResConfig::getInstance()->getUndoButtonRes());
	if (!buttonBg) {
		// 如果没有按钮资源，使用默认精灵
		buttonBg = Sprite::create();
		if (buttonBg) {
			buttonBg->setContentSize(Size(120, 60));
			buttonBg->setColor(Color3B::GRAY);
		}
	}

	// 创建撤销按钮文字
	auto buttonLabel = Label::createWithTTF("Undo", CardResConfig::getInstance()->getFontRes(), 20);
	if (buttonLabel) {
		buttonLabel->setPosition(buttonBg->getContentSize() / 2);
		buttonBg->addChild(buttonLabel);
	}

	// 创建菜单项
	_undoButton = MenuItemImage::create(buttonBg->getTexture()->getPath(),
		buttonBg->getTexture()->getPath(),
		CC_CALLBACK_1(GameView::onUndoButtonClick, this));

	if (_undoButton) {
		_undoButton->setPosition(Vec2(800, 500));

		// 创建菜单
		auto menu = Menu::create(_undoButton, nullptr);
		menu->setPosition(Vec2::ZERO);
		addChild(menu);
	}
}

void GameView::createScoreAndMoveDisplay() {
	// 创建分数显示
	_scoreLabel = Label::createWithTTF("Score: 0", CardResConfig::getInstance()->getFontRes(), 24);
	if (_scoreLabel) {
		_scoreLabel->setPosition(Vec2(100, 600));
		_scoreLabel->setColor(Color3B::WHITE);
		addChild(_scoreLabel);
	}

	// 创建步数显示
	_moveCountLabel = Label::createWithTTF("Moves: 0", CardResConfig::getInstance()->getFontRes(), 24);
	if (_moveCountLabel) {
		_moveCountLabel->setPosition(Vec2(300, 600));
		_moveCountLabel->setColor(Color3B::WHITE);
		addChild(_moveCountLabel);
	}
}

void GameView::createGameStateDisplay() {
	// 创建游戏状态显示
	_gameStateLabel = Label::createWithTTF("Playing", CardResConfig::getInstance()->getFontRes(), 32);
	if (_gameStateLabel) {
		_gameStateLabel->setPosition(Vec2(500, 600));
		_gameStateLabel->setColor(Color3B::GREEN);
		addChild(_gameStateLabel);
	}
}

void GameView::onUndoButtonClick(cocos2d::Ref* sender) {
	// 处理撤销按钮点击
	if (_undoButtonClickCallback) {
		_undoButtonClickCallback();
	}
}