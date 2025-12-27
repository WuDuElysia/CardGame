#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "cocos2d.h"
#include "views/PlayFieldView.h"
#include "views/StackView.h"

/**
 * @file GameView.h
 * @brief 游戏主视图
 * @details 游戏主视图，管理主牌区和牌堆等游戏界面元素，处理游戏状态变化、动画播放等
 */
class GameView : public cocos2d::Node {
public:
	typedef std::function<void()> OnUndoButtonClickCallback;

	static GameView* create();
	virtual bool init();

	// 获取主牌区视图
	PlayFieldView* getPlayFieldView() const;

	// 获取牌堆视图
	StackView* getStackView() const;

	// 初始化游戏UI
	void initUI();

	// 播放入场动画
	void playEntranceAnimation();

	// 更新游戏UI
	void updateGameUI();

	// 设置撤销按钮点击回调
	void setUndoButtonClickCallback(const OnUndoButtonClickCallback& callback);

	// 更新分数显示
	void updateScore(int score);

	// 更新步数显示
	void updateMoveCount(int moveCount);

	// 设置游戏状态
	void setGameState(int gameState);

private:
	// 创建撤销按钮
	void createUndoButton();

	// 创建分数和步数显示
	void createScoreAndMoveDisplay();

	// 创建游戏状态显示
	void createGameStateDisplay();

	// 处理撤销按钮点击
	void onUndoButtonClick(cocos2d::Ref* sender);

	PlayFieldView* _playFieldView;
	StackView* _stackView;

	// 撤销按钮
	cocos2d::MenuItemImage* _undoButton;

	// 分数显示
	cocos2d::Label* _scoreLabel;

	// 步数显示
	cocos2d::Label* _moveCountLabel;

	// 游戏状态显示
	cocos2d::Label* _gameStateLabel;

	// 撤销按钮点击回调
	OnUndoButtonClickCallback _undoButtonClickCallback;
};

#endif