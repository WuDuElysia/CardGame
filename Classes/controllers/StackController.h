#ifndef STACK_CONTROLLER_H
#define STACK_CONTROLLER_H

#include "models/GameModel.h"
#include "views/StackView.h"
#include "managers/UndoManager.h"
#include "managers/CardStackManager.h"

/**
 * @file StackController.h
 * @brief 牌堆控制器
 * @details 处理备用牌堆和底牌堆的交互
 */
class StackController {
public:
    /**
     * @brief 构造函数
     * @param gameModel 游戏模型指针
     * @param stackView 牌堆视图指针
     */
    StackController(GameModel* gameModel, StackView* stackView);
    
    /**
     * @brief 析构函数
     */
    virtual ~StackController();
    
    /**
     * @brief 初始化控制器
     * @return 是否初始化成功
     */
    bool init();
    
    /**
     * @brief 处理牌堆点击事件
     * @return 是否处理成功
     */
    bool handleStackClick();
    
    /**
     * @brief 处理底牌点击事件
     * @param cardId 卡牌ID
     * @return 是否处理成功
     */
    bool handleTrayClick(int cardId);
    
    /**
     * @brief 从牌堆抽取卡牌替换底牌
     * @return 是否替换成功
     */
    bool replaceTrayWithStackCard();
    
    /**
     * @brief 检查牌堆是否为空
     * @return 是否为空
     */
    bool isStackEmpty() const;
    
    /**
     * @brief 设置撤销管理器
     * @param undoManager 撤销管理器指针
     */
    void setUndoManager(UndoManager* undoManager);
    
private:
    GameModel* _gameModel;             // 游戏模型指针
    StackView* _stackView;             // 牌堆视图指针
    UndoManager* _undoManager;         // 撤销管理器指针
    CardStackManager* _cardStackManager;  // 牌堆管理器指针
};

#endif // __STACK_CONTROLLER_H__
