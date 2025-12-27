#include "GameModelFromLevelGenerator.h"
#include "models/CardModel.h"
#include "utils/CardEnums.h"

GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig& levelConfig) {
    GameModel* gameModel = new GameModel();
    gameModel->init();
    
    // 生成主牌堆卡牌
    generatePlayfieldCards(gameModel, levelConfig);
    
    // 生成备用牌堆卡牌
    generateStackCards(gameModel, levelConfig);
    
    // 初始化底牌
    initializeTrayCard(gameModel);
    
    return gameModel;
}

void GameModelFromLevelGenerator::generatePlayfieldCards(GameModel* gameModel, const LevelConfig& levelConfig) {
    // 从关卡配置中读取主牌堆卡牌数据并创建CardModel
    const auto& playfieldCards = levelConfig.getPlayfieldCards();
    for (int i = 0; i < playfieldCards.size(); ++i) {
        const auto& cardData = playfieldCards[i];
        
        CardModel* cardModel = new CardModel(
            i + 1,  // cardId
            cardData.cardFace,
            cardData.cardSuit,
            cocos2d::Vec2(cardData.position.x, cardData.position.y)
        );
        
        gameModel->addPlayfieldCard(cardModel);
    }
}

void GameModelFromLevelGenerator::generateStackCards(GameModel* gameModel, const LevelConfig& levelConfig) {
    // 从关卡配置中读取备用牌堆卡牌数据并创建CardModel
    const auto& stackCards = levelConfig.getStackCards();
    for (int i = 0; i < stackCards.size(); ++i) {
        const auto& cardData = stackCards[i];
        
        CardModel* cardModel = new CardModel(
            stackCards.size() + i + 1,  // cardId，确保不与主牌堆冲突
            cardData.cardFace,
            cardData.cardSuit,
            cocos2d::Vec2::ZERO  // 备用牌堆位置由视图层处理
        );
        
        gameModel->addStackCard(cardModel);
    }
}

void GameModelFromLevelGenerator::initializeTrayCard(GameModel* gameModel) {
    // 从备用牌堆抽取第一张牌作为初始底牌
    if (!gameModel->getStackCards().empty()) {
        CardModel* trayCard = gameModel->drawStackCard();
        if (trayCard != nullptr) {
            gameModel->setTrayCard(trayCard);
        }
    }
}