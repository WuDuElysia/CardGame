#ifndef __PLAY_FIELD_VIEW_H
#define __PLAY_FIELD_VIEW_H

#include "cocos2d.h"
#include "views/CardView.h"

class PlayFieldView : public cocos2d::Node {
public:
	typedef std::function<void(int)> OnCardClickCallback;

	static std::shared_ptr<PlayFieldView> create();
	virtual bool init();

	void addCardView(CardView* cardView);
	void removeCardView(CardView* cardView);
	std::shared_ptr<CardView> getCardViewByCardId(int cardId) const;

	// 设置卡牌点击回调
	void setCardClickCallback(const OnCardClickCallback& callback);

private:
	bool onMouseDown(cocos2d::Event* event);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	// 根据位置查找点击的卡牌
	std::shared_ptr<CardView> findCardAtPosition(const cocos2d::Vec2& pos) const;

	std::vector<std::shared_ptr<CardView>> _cardViews;
	OnCardClickCallback _onCardClickCallback;
};

#endif