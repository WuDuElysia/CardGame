#ifndef CARD_STACK_MANAGER_H
#define CARD_STACK_MANAGER_H

#include "models/GameModel.h"

/**
 * @file CardStackManager.h
 * @brief 牌堆管理器
 * @details 管理备用牌堆和底牌堆
 */
class CardStackManager {
public:
    CardStackManager();
    virtual ~CardStackManager();
    
    /**
     * @brief 初始化牌堆管理器
     * @param gameModel 游戏模型指针
     * @return 是否初始化成功
     */
    bool init(GameModel* gameModel);
    
    /**
     * @brief 从备用牌堆抽取一张卡牌
     * @return 抽取的卡牌指针，失败返回nullptr
     */
    CardModel* drawCardFromStack();
    
    /**
     * @brief 向备用牌堆添加一张卡牌
     * @param card 卡牌指针
     * @return 是否添加成功
     */
    bool addCardToStack(CardModel* card);
    
    /**
     * @brief 检查备用牌堆是否为空
     * @return 是否为空
     */
    bool isStackEmpty() const;
    
    /**
     * @brief 替换当前底牌
     * @param card 新的底牌指针
     * @return 是否替换成功
     */
    bool replaceTrayCard(CardModel* card);
    
    /**
     * @brief 从备用牌堆抽取一张卡牌替换当前底牌
     * @return 是否替换成功
     */
    bool replaceTrayFromStack();
    
    /**
     * @brief 获取当前底牌
     * @return 当前底牌指针，没有则返回nullptr
     */
    CardModel* getCurrentTrayCard() const;
    
    /**
     * @brief 获取备用牌堆卡牌数量
     * @return 卡牌数量
     */
    int getStackCardCount() const;
    
private:
    GameModel* _gameModel;  ///< 游戏模型指针
};

#endif // __CARD_STACK_MANAGER_H__

