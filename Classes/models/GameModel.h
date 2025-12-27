#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include <vector>
#include <map>
#include "cocos2d.h"
#include "models/CardModel.h"

/**
 * @file GameModel.h
 * @brief 游戏数据模型类
 * @details 游戏数据模型，存储游戏的核心数据和状态，管理游戏状态、游戏区域卡片、底牌、牌堆等信息
 * @author [Author Name]
 * @date [Date]
 */
class GameModel {
public:
        GameModel();
        virtual ~GameModel();
        bool init();

        // 游戏状态枚举
        enum GameState {
                GAME_STATE_IDLE = 0,
                GAME_STATE_PLAYING,
                GAME_STATE_PAUSED,
                GAME_STATE_WIN,
                GAME_STATE_LOSE
        };

        // 游戏区域相关
        const std::vector<std::shared_ptr<CardModel>>& getPlayfieldCards() const;
        void addPlayfieldCard(CardModel* card);
        void removePlayfieldCard(int cardId);
        CardModel* getPlayfieldCardById(int cardId) const;

        const std::vector<std::shared_ptr<CardModel>>& getTrayCards() const;
        void addTrayCard(CardModel* card);
        void removeTrayCard(int cardId);
        CardModel* getTrayCardById(int cardId) const;
        CardModel* getTrayCard() const;  // 获取当前底牌

        const std::vector<std::shared_ptr<CardModel>>& getStackCards() const;
        void addStackCard(CardModel* card);
        void removeStackCard(int cardId);
        CardModel* getStackCardById(int cardId) const;
        CardModel* drawStackCard();
        bool isStackEmpty() const;

        // 游戏逻辑
        bool canMatchCard(const CardModel* card) const;
        bool matchCard(CardModel* card);
        bool replaceTrayWithStackCard();
        bool checkWinCondition() const;
        bool checkLoseCondition() const;

        // 游戏状态
        int getGameState() const;
        void setGameState(int state);

        // 获取游戏统计信息
        int getScore() const;
        void setScore(int score);
        int getMoveCount() const;
        void incrementMoveCount();

private:
        // 游戏区域
        std::vector<std::shared_ptr<CardModel>> _playfieldCards;
        std::map<int, std::shared_ptr<CardModel>> _playfieldCardMap;
        
        // 底牌堆
        std::vector<std::shared_ptr<CardModel>> _trayCards;
        std::map<int, std::shared_ptr<CardModel>> _trayCardMap;
        
        // 备用牌堆
        std::vector<std::shared_ptr<CardModel>> _stackCards;
        std::map<int, std::shared_ptr<CardModel>> _stackCardMap;

        int _gameState;
        int _score;
        int _moveCount;
};

#endif