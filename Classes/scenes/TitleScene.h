#ifndef TITLE_SCENE_H
#define TITLE_SCENE_H

#include "cocos2d.h"

/**
 * @file TitleScene.h
 * @brief 游戏标题场景
 * @details 游戏标题场景，显示游戏名称和开始游戏按钮
 */
class TitleScene : public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(TitleScene);

private:
	// 处理开始游戏按钮点击
	void onStartGameButtonClick(cocos2d::Ref* pSender);
};

#endif // TITLE_SCENE_H