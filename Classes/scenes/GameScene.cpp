#include "GameScene.h"
#include "cocos2d.h"

USING_NS_CC;

Scene* GameScene::createScene() {
	return GameScene::create();
}

bool GameScene::init() {
	if (!Scene::init()) {
		return false;
	}

	// 初始化成员变量
	_gameController = nullptr;
	_gameView = nullptr;

	// 创建游戏视图
	_gameView = GameView::create();
	if (_gameView) {
		addChild(_gameView);
	}

	// 创建游戏控制器
	_gameController = new GameController();
	if (_gameController) {
		// 初始化控制器
		if (_gameController->init()) {
			// 设置游戏视图
			_gameController->setGameView(_gameView);

			// 开始游戏（关卡ID为1）
			_gameController->startGame(1);
		}
	}

	// 添加键盘事件监听器，绑定R键到撤销功能
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
		if (keyCode == EventKeyboard::KeyCode::KEY_R) {
			// 调用游戏控制器的撤销方法
			if (_gameController) {
				_gameController->undo();
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	return true;
}



