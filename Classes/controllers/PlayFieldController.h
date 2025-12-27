#ifndef PLAY_FIELD_CONTROLLER_H
#define PLAY_FIELD_CONTROLLER_H

#include "models/GameModel.h"
#include "views/PlayFieldView.h"
#include "managers/UndoManager.h"
#include "managers/PlayFieldManager.h"
#include "managers/CardStackManager.h"

class PlayFieldController {
public:
	PlayFieldController(GameModel* gameModel, PlayFieldView* playFieldView);
	virtual ~PlayFieldController();

	bool init();

	// 处理卡牌点击事件
	bool handleCardClick(int cardId);

	// 使用游戏区域卡牌替换底牌
	bool replaceTrayWithPlayFieldCard(int cardId);

	// 从牌堆获取卡牌替换底牌
	bool replaceTrayWithStackCard();

	// 设置撤销管理器
    void setUndoManager(UndoManager* undoManager);

private:
    // 检查卡牌是否可以移动
    bool canMoveCard(int cardId) const;

	GameModel* _gameModel;
	PlayFieldView* _playFieldView;
	UndoManager* _undoManager;
	PlayFieldManager* _playFieldManager;
	CardStackManager* _cardStackManager;
};

#endif