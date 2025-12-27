#ifndef PLAY_FIELD_VIEW_H
#define PLAY_FIELD_VIEW_H

#include "cocos2d.h"
#include "views/CardView.h"

class PlayFieldView : public cocos2d::Node {
public:
	typedef std::function<void(int)> OnCardClickCallback;

	static PlayFieldView* create();
	virtual bool init();

	// 卡牌管理
	void addCardView(CardView* cardView);
	void removeCardView(CardView* cardView);
	void removeCardViewByCardId(int cardId);
	CardView* getCardViewByCardId(int cardId) const;

	// 卡牌动画
	void moveCardToPosition(CardView* cardView, const cocos2d::Vec2& position, float duration = 0.3f);
	void flipCard(CardView* cardView, float duration = 0.5f);
	void scaleCard(CardView* cardView, const cocos2d::Vec2& scale, float duration = 0.2f);
	void highlightCard(CardView* cardView, bool highlight, float duration = 0.2f);

	// 堆叠显示设置
	void setCardStackPosition(const cocos2d::Vec2& basePosition, float offset = 20.0f);
	void updateStackDisplay();
	void setTopCardVisible(bool visible);

	// 空状态显示
	void setEmptyStateVisible(bool visible);

	// 回调设置
	void setCardClickCallback(const OnCardClickCallback& callback);

private:
	// 事件处理
	bool onMouseDown(cocos2d::Event* event);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	// 根据位置查找目标卡牌
	CardView* findCardAtPosition(const cocos2d::Vec2& pos) const;

	// 成员变量
	std::vector<CardView*> _cardViews;
	OnCardClickCallback _onCardClickCallback;
	
	// 堆叠显示相关
	cocos2d::Vec2 _stackBasePosition;
	float _stackOffset;
	bool _topCardVisible;
	
	// 空状态显示
	cocos2d::Sprite* _emptyStateSprite;
	cocos2d::Label* _emptyStateLabel;
};

#endif