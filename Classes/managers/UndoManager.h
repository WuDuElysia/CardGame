#ifndef UNDO_MANAGER_H
#define UNDO_MANAGER_H

#include <vector>
#include "models/GameModel.h"
#include "models/UndoModel.h"

/**
 * @class UndoManager
 * @brief 撤销管理器类
 * @details 负责记录游戏中的操作历史，实现游戏操作的撤销功能
 *          管理撤销历史的最大步数和清除操作
 */
class UndoManager {
public:
    /**
     * @brief 构造函数
     */
    UndoManager();
    
    /**
     * @brief 析构函数
     */
    virtual ~UndoManager();
    
    /**
     * @brief 初始化撤销管理器
     * @param gameModel 游戏数据模型指针
     * @return 是否初始化成功
     */
    bool init(GameModel* gameModel);
    
    /**
     * @brief 记录撤销操作
     * @param undoAction 要记录的撤销操作数据
     */
    void recordUndoAction(const UndoModel& undoAction);
    
    /**
     * @brief 执行撤销操作
     * @return 是否撤销成功
     */
    bool undo();
    
    /**
     * @brief 检查是否可以执行撤销操作
     * @return 是否可以撤销
     */
    bool canUndo() const;
    
    /**
     * @brief 获取当前撤销历史中的操作数量
     * @return 撤销操作数量
     */
    int getUndoCount() const;
    
    /**
     * @brief 清空撤销历史
     */
    void clearUndoHistory();
    
    /**
     * @brief 设置最大撤销步数
     * @param maxSteps 最大撤销步数
     */
    void setMaxUndoSteps(int maxSteps);
    
    /**
     * @brief 获取最大撤销步数
     * @return 最大撤销步数
     */
    int getMaxUndoSteps() const;

private:
    GameModel* _gameModel;             ///< 游戏数据模型指针
    std::vector<UndoModel> _undoHistory; ///< 撤销操作历史记录
    int _maxUndoSteps;                 ///< 最大撤销步数限制
};

#endif // __UNDO_MANAGER_H__

