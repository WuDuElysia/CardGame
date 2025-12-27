#ifndef __GAME_MODEL_FROM_LEVEL_GENERATOR_H__
#define __GAME_MODEL_FROM_LEVEL_GENERATOR_H__

#include "models/GameModel.h"
#include "configs/models/LevelConfig.h"

class GameModelFromLevelGenerator {
public:
    static GameModel* generateGameModel(const LevelConfig& levelConfig);
    
private:
    // 生成主牌堆卡牌
    static void generatePlayfieldCards(GameModel* gameModel, const LevelConfig& levelConfig);
    
    // 生成备用牌堆卡牌
    static void generateStackCards(GameModel* gameModel, const LevelConfig& levelConfig);
    
    // 初始化底牌
    static void initializeTrayCard(GameModel* gameModel);
};

#endif // __GAME_MODEL_FROM_LEVEL_GENERATOR_H__