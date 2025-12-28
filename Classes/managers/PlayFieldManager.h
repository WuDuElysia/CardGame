#ifndef PLAY_FIELD_MANAGER_H
#define PLAY_FIELD_MANAGER_H

#include "models/GameModel.h"

/**
 * @file PlayFieldManager.h
 * @brief 主牌区管理器
 * @details 管理主牌区的卡牌
 */
class PlayFieldManager {
public:
    PlayFieldManager();
    virtual ~PlayFieldManager();
    
    /**
     * @brief 初始化主牌区管理器
     * @param gameModel 游戏模型指针
     * @return 是否初始化成功
     */
    bool init(GameModel* gameModel);
    
    /**
     * @brief 检查卡牌是否可以匹配
     * @param cardId 卡牌ID
     * @return 是否可以匹配
     */
    bool canMatchCard(int cardId) const;
    
    /**
     * @brief 匹配卡牌
     * @param cardId 卡牌ID
     * @return 是否匹配成功
     */
    bool matchCard(int cardId);
    
    /**
     * @brief 根据ID获取卡牌
     * @param cardId 卡牌ID
     * @return 卡牌指针，失败返回nullptr
     */
    CardModel* getCardById(int cardId) const;
    
    /**
     * @brief 移除指定卡牌
     * @param cardId 卡牌ID
     */
    void removeCard(int cardId);
    
    /**
     * @brief 向主牌区添加一张卡牌
     * @param card 卡牌指针
     * @return 是否添加成功
     */
    bool addCard(CardModel* card);
    
    /**
     * @brief 检查主牌区是否为空
     * @return 是否为空
     */
    bool isPlayFieldEmpty() const;
    
private:
    GameModel* _gameModel;  ///< 游戏模型指针
};

#endif // __PLAY_FIELD_MANAGER_H__

