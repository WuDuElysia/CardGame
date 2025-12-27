#include "TitleScene.h"
#include "GameScene.h"
#include "cocos2d.h"

USING_NS_CC;

Scene* TitleScene::createScene() {
	return TitleScene::create();
}

bool TitleScene::init() {
	if (!Scene::init()) {
		return false;
	}

	// 获取屏幕大小
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 设置背景颜色为深蓝色
	auto layerColor = LayerColor::create(Color4B(50, 50, 100, 255));
	this->addChild(layerColor);

	// 创建标题文字
	auto titleLabel = Label::createWithTTF("CardGame", "fonts/Marker Felt.ttf", 64);
	titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.7));
	titleLabel->setColor(Color3B(255, 255, 255));
	this->addChild(titleLabel, 1);

	// 创建开始游戏按钮
	auto startButton = MenuItemFont::create("start game", CC_CALLBACK_1(TitleScene::onStartGameButtonClick, this));
	startButton->setFontSizeObj(48);
	startButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.4));
	startButton->setColor(Color3B(255, 255, 255));

	// 创建菜单并添加按钮
	auto menu = Menu::create(startButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

void TitleScene::onStartGameButtonClick(cocos2d::Ref* pSender) {
	// 切换到游戏场景
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}