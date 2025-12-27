#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "cocos2d.h"
#include "../views/GameView.h"
#include "../controllers/GameController.h"

/**
 * @file GameScene.h
 * @brief 游戏主场景
 * @details 游戏主场景，负责初始化游戏控制器和视图，管理游戏流程
 */
class GameScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
private:
    GameController* _gameController;
    GameView* _gameView;
};

#endif // GAME_SCENE_H