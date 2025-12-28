#ifndef PLAY_FIELD_CONTROLLER_H
#define PLAY_FIELD_CONTROLLER_H

#include "models/GameModel.h"
#include "views/PlayFieldView.h"
#include "managers/UndoManager.h"
#include "managers/PlayFieldManager.h"
#include "managers/CardStackManager.h"

// 前向声明：避免循环依赖
class StackView;

/**
 * @class PlayFieldController
 * @brief 主牌区控制器类
 * @details 负责处理主牌区的交互逻辑，协调主牌区模型和视图的更新
 *          管理主牌区卡牌的点击、匹配、移动等操作
 */
class PlayFieldController {
public:
	/**
	 * @brief 构造函数
	 * @param gameModel 游戏数据模型指针
	 * @param playFieldView 主牌区视图指针
	 * @param stackView 牌堆视图指针，用于更新底牌显示
	 */
	PlayFieldController(GameModel* gameModel, PlayFieldView* playFieldView, StackView* stackView);
	
	/**
	 * @brief 析构函数
	 * @details 释放控制器占用的资源
	 */
	virtual ~PlayFieldController();

	/**
	 * @brief 初始化控制器
	 * @return 初始化是否成功
	 */
	bool init();

	/**
	 * @brief 处理卡牌点击事件
	 * @param cardId 被点击的卡牌ID
	 * @return 点击处理是否成功
	 */
	bool handleCardClick(int cardId);

	/**
	 * @brief 使用游戏区域卡牌替换底牌
	 * @param cardId 要替换底牌的卡牌ID
	 * @return 替换是否成功
	 */
	bool replaceTrayWithPlayFieldCard(int cardId);

	/**
	 * @brief 从牌堆获取卡牌替换底牌
	 * @return 替换是否成功
	 */
	bool replaceTrayWithStackCard();

	/**
	 * @brief 设置撤销管理器
	 * @param undoManager 撤销管理器指针
	 */
	void setUndoManager(UndoManager* undoManager);

	/**
	 * @brief 重新初始化UI
	 * @details 用于撤销操作后更新UI，确保视图与数据模型一致
	 */
	void reinitUI();

private:
	/**
	 * @brief 检查卡牌是否可以移动
	 * @param cardId 要检查的卡牌ID
	 * @return 卡牌是否可以移动
	 */
	bool canMoveCard(int cardId) const;

	GameModel* _gameModel;         ///< 游戏数据模型指针
	PlayFieldView* _playFieldView; ///< 主牌区视图指针
	StackView* _stackView;         ///< 牌堆视图指针，用于更新底牌显示
	UndoManager* _undoManager;     ///< 撤销管理器指针
	PlayFieldManager* _playFieldManager; ///< 主牌区管理器指针
	CardStackManager* _cardStackManager; ///< 牌堆管理器指针
};

#endif
