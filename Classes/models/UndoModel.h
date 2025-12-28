#ifndef UNDO_MODEL_H
#define UNDO_MODEL_H

#include "cocos2d.h"
#include "models/CardEnums.h"

/**
 * @file UndoModel.h
 * @brief 撤销操作数据模型
 * @details 用于存储游戏操作的撤销数据，支持游戏状态回退
 */
class UndoModel {
public:
    UndoModel();
    virtual ~UndoModel();
    
    /**
     * @brief 设置操作类型
     * @param actionType 操作类型（如"moveCard"、"replaceTray"）
     */
    void setActionType(const std::string& actionType);
    
    /**
     * @brief 获取操作类型
     * @return 操作类型
     */
    const std::string& getActionType() const;
    
    /**
     * @brief 设置操作涉及的卡牌ID
     * @param cardId 卡牌ID
     */
    void setCardId(int cardId);
    
    /**
     * @brief 获取操作涉及的卡牌ID
     * @return 卡牌ID
     */
    int getCardId() const;
    
    /**
     * @brief 设置卡牌原始位置
     * @param position 原始位置
     */
    void setFromPosition(const cocos2d::Vec2& position);
    
    /**
     * @brief 获取卡牌原始位置
     * @return 原始位置
     */
    const cocos2d::Vec2& getFromPosition() const;
    
    /**
     * @brief 设置卡牌目标位置
     * @param position 目标位置
     */
    void setToPosition(const cocos2d::Vec2& position);
    
    /**
     * @brief 获取卡牌目标位置
     * @return 目标位置
     */
    const cocos2d::Vec2& getToPosition() const;
    
    /**
     * @brief 设置操作前的卡牌花色
     * @param suit 花色
     */
    void setOldSuit(CardSuitType suit);
    
    /**
     * @brief 获取操作前的卡牌花色
     * @return 花色
     */
    CardSuitType getOldSuit() const;
    
    /**
     * @brief 设置操作前的卡牌点数
     * @param face 点数
     */
    void setOldFace(CardFaceType face);
    
    /**
     * @brief 获取操作前的卡牌点数
     * @return 点数
     */
    CardFaceType getOldFace() const;
    
    /**
     * @brief 设置操作后的卡牌花色
     * @param suit 花色
     */
    void setNewSuit(CardSuitType suit);
    
    /**
     * @brief 获取操作后的卡牌花色
     * @return 花色
     */
    CardSuitType getNewSuit() const;
    
    /**
     * @brief 设置操作后的卡牌点数
     * @param face 点数
     */
    void setNewFace(CardFaceType face);
    
    /**
     * @brief 获取操作后的卡牌点数
     * @return 点数
     */
    CardFaceType getNewFace() const;
    
    /**
     * @brief 设置操作前的牌堆状态
     * @param stackCardCount 牌堆卡牌数量
     */
    void setOldStackCardCount(int stackCardCount);
    
    /**
     * @brief 获取操作前的牌堆状态
     * @return 牌堆卡牌数量
     */
    int getOldStackCardCount() const;
    
    /**
     * @brief 设置操作后的牌堆状态
     * @param stackCardCount 牌堆卡牌数量
     */
    void setNewStackCardCount(int stackCardCount);
    
    /**
     * @brief 获取操作后的牌堆状态
     * @return 牌堆卡牌数量
     */
    int getNewStackCardCount() const;

private:
    std::string _actionType;       ///< 操作类型
    int _cardId;                   ///< 操作涉及的卡牌ID
    cocos2d::Vec2 _fromPosition;   ///< 卡牌原始位置
    cocos2d::Vec2 _toPosition;     ///< 卡牌目标位置
    CardSuitType _oldSuit;         ///< 操作前的卡牌花色
    CardFaceType _oldFace;         ///< 操作前的卡牌点数
    CardSuitType _newSuit;         ///< 操作后的卡牌花色
    CardFaceType _newFace;         ///< 操作后的卡牌点数
    int _oldStackCardCount;        ///< 操作前的牌堆卡牌数量
    int _newStackCardCount;        ///< 操作后的牌堆卡牌数量
};

#endif // __UNDO_MODEL_H__

