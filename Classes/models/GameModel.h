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
        /**
         * @brief 构造函数
         */
        GameModel();
        
        /**
         * @brief 析构函数
         */
        virtual ~GameModel();
        
        /**
         * @brief 初始化游戏模型
         * @return 是否初始化成功
         */
        bool init();

        // 游戏状态枚举
        enum GameState {
                GAME_STATE_IDLE = 0,    ///< 游戏空闲状态
                GAME_STATE_PLAYING,     ///< 游戏进行中
                GAME_STATE_PAUSED,      ///< 游戏暂停
                GAME_STATE_WIN,         ///< 游戏胜利
                GAME_STATE_LOSE         ///< 游戏失败
        };

        // 游戏区域相关
        /**
         * @brief 获取主牌区卡牌列表
         * @return 主牌区卡牌的常量引用
         */
        const std::vector<std::shared_ptr<CardModel>>& getPlayfieldCards() const;
        
        /**
         * @brief 向主牌区添加卡牌
         * @param card 要添加的卡牌指针
         */
        void addPlayfieldCard(CardModel* card);
        
        /**
         * @brief 从主牌区移除卡牌
         * @param cardId 要移除的卡牌ID
         */
        void removePlayfieldCard(int cardId);
        
        /**
         * @brief 根据ID获取主牌区卡牌
         * @param cardId 卡牌ID
         * @return 卡牌指针，若未找到则返回nullptr
         */
        CardModel* getPlayfieldCardById(int cardId) const;

        /**
         * @brief 获取底牌堆卡牌列表
         * @return 底牌堆卡牌的常量引用
         */
        const std::vector<std::shared_ptr<CardModel>>& getTrayCards() const;
        
        /**
         * @brief 向底牌堆添加卡牌
         * @param card 要添加的卡牌指针
         */
        void addTrayCard(CardModel* card);
        
        /**
         * @brief 从底牌堆移除卡牌
         * @param cardId 要移除的卡牌ID
         */
        void removeTrayCard(int cardId);
        
        /**
         * @brief 根据ID获取底牌堆卡牌
         * @param cardId 卡牌ID
         * @return 卡牌指针，若未找到则返回nullptr
         */
        CardModel* getTrayCardById(int cardId) const;
        
        /**
         * @brief 获取当前底牌
         * @return 当前底牌指针，若底牌堆为空则返回nullptr
         */
        CardModel* getTrayCard() const;
        
        /**
         * @brief 将底牌移动到主牌区
         * @param cardId 要移动的卡牌ID
         * @return 是否移动成功
         */
        bool moveTrayCardToPlayfield(int cardId);

        /**
         * @brief 获取备用牌堆卡牌列表
         * @return 备用牌堆卡牌的常量引用
         */
        const std::vector<std::shared_ptr<CardModel>>& getStackCards() const;
        
        /**
         * @brief 向备用牌堆添加卡牌
         * @param card 要添加的卡牌指针
         */
        void addStackCard(CardModel* card);
        
        /**
         * @brief 从备用牌堆移除卡牌
         * @param cardId 要移除的卡牌ID
         */
        void removeStackCard(int cardId);
        
        /**
         * @brief 根据ID获取备用牌堆卡牌
         * @param cardId 卡牌ID
         * @return 卡牌指针，若未找到则返回nullptr
         */
        CardModel* getStackCardById(int cardId) const;
        
        /**
         * @brief 从备用牌堆抽取顶部卡牌
         * @return 抽取的卡牌指针，若牌堆为空则返回nullptr
         */
        CardModel* drawStackCard();
        
        /**
         * @brief 检查备用牌堆是否为空
         * @return 牌堆是否为空
         */
        bool isStackEmpty() const;

        // 游戏逻辑
        /**
         * @brief 检查卡牌是否可以与当前底牌匹配
         * @param card 要检查的卡牌指针
         * @return 是否可以匹配
         */
        bool canMatchCard(const CardModel* card) const;
        
        /**
         * @brief 将卡牌与当前底牌匹配
         * @param card 要匹配的卡牌指针
         * @return 是否匹配成功
         */
        bool matchCard(CardModel* card);
        
        /**
         * @brief 从备用牌堆抽取卡牌替换底牌
         * @return 是否替换成功
         */
        bool replaceTrayWithStackCard();
        
        /**
         * @brief 检查游戏胜利条件
         * @return 是否满足胜利条件
         */
        bool checkWinCondition() const;
        
        /**
         * @brief 检查游戏失败条件
         * @return 是否满足失败条件
         */
        bool checkLoseCondition() const;

        // 游戏状态
        /**
         * @brief 获取当前游戏状态
         * @return 当前游戏状态枚举值
         */
        int getGameState() const;
        
        /**
         * @brief 设置游戏状态
         * @param state 要设置的游戏状态
         */
        void setGameState(int state);

        // 获取游戏统计信息
        /**
         * @brief 获取当前分数
         * @return 当前分数
         */
        int getScore() const;
        
        /**
         * @brief 设置分数
         * @param score 要设置的分数
         */
        void setScore(int score);
        
        /**
         * @brief 获取当前步数
         * @return 当前步数
         */
        int getMoveCount() const;
        
        /**
         * @brief 增加步数
         */
        void incrementMoveCount();

private:
        // 游戏区域
        std::vector<std::shared_ptr<CardModel>> _playfieldCards;    ///< 主牌区卡牌列表
        std::map<int, std::shared_ptr<CardModel>> _playfieldCardMap; ///< 主牌区卡牌映射表
        
        // 底牌堆
        std::vector<std::shared_ptr<CardModel>> _trayCards;         ///< 底牌堆卡牌列表
        std::map<int, std::shared_ptr<CardModel>> _trayCardMap;     ///< 底牌堆卡牌映射表
        
        // 备用牌堆
        std::vector<std::shared_ptr<CardModel>> _stackCards;        ///< 备用牌堆卡牌列表
        std::map<int, std::shared_ptr<CardModel>> _stackCardMap;    ///< 备用牌堆卡牌映射表

        int _gameState;     ///< 当前游戏状态
        int _score;         ///< 当前分数
        int _moveCount;     ///< 当前步数
};

#endif

