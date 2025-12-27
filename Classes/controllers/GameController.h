#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "models/GameModel.h"
#include "views/GameView.h"
#include "controllers/PlayFieldController.h"
#include "controllers/StackController.h"
#include "managers/UndoManager.h"

/**
 * @file GameController.h
 * @brief 游戏主控制器
 * @details 管理整个游戏流程，协调各子控制器
 */
class GameController {
public:
	GameController();
	virtual ~GameController();

	/**
	 * @brief 初始化游戏控制器
	 * @return 是否初始化成功
	 */
	bool init();

	/**
	 * @brief 开始游戏
	 * @param levelId 关卡ID
	 * @return 是否开始成功
	 */
	bool startGame(int levelId);

	/**
	 * @brief 暂停游戏
	 */
	void pauseGame();

	/**
	 * @brief 恢复游戏
	 */
	void resumeGame();

	/**
	 * @brief 结束游戏
	 */
	void endGame();

	/**
	 * @brief 执行撤销操作
	 * @return 是否撤销成功
	 */
	bool undo();

	/**
	 * @brief 设置游戏视图
	 * @param gameView 游戏视图指针
	 */
	void setGameView(GameView* gameView);

	/**
	 * @brief 获取游戏模型
	 * @return 游戏模型指针
	 */
	GameModel* getGameModel() const;

	/**
	 * @brief 获取主牌区控制器
	 * @return 主牌区控制器指针
	 */
	PlayFieldController* getPlayFieldController() const;

	/**
	 * @brief 获取牌堆控制器
	 * @return 牌堆控制器指针
	 */
	StackController* getStackController() const;

	/**
	 * @brief 获取撤销管理器
	 * @return 撤销管理器指针
	 */
	UndoManager* getUndoManager() const;

private:
	/**
	 * @brief 初始化子控制器
	 * @return 是否初始化成功
	 */
	bool initControllers();

	/**
	 * @brief 初始化撤销管理器
	 * @return 是否初始化成功
	 */
	bool initUndoManager();

	/**
	 * @brief 加载关卡配置
	 * @param levelId 关卡ID
	 * @return 是否加载成功
	 */
	bool loadLevel(int levelId);

	GameModel* _gameModel;                  // 游戏模型
	GameView* _gameView;                    // 游戏视图
	PlayFieldController* _playFieldController;  // 主牌区控制器
	StackController* _stackController;      // 牌堆控制器
	UndoManager* _undoManager;              // 撤销管理器
	int _currentLevelId;                    // 当前关卡ID
	int _score;                             // 当前分数
	int _moveCount;                         // 当前步数
};

#endif // GAME_CONTROLLER_H
