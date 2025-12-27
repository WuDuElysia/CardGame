#ifndef __CARDMODEL_H
#define __CARDMODEL_H

#include "CardEnums.h"
#include "cocos2d.h"

class CardModel {
public:
	CardModel();
	CardModel(int cardId, CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position);
	virtual ~CardModel();

	int getCardId() const;              // 获取卡牌ID
	CardSuitType getCardSuit() const;   // 获取卡牌花色
	CardFaceType getCardFace() const;   // 获取卡牌点数
	const cocos2d::Vec2& getPosition() const;  // 获取卡牌位置
	void setPosition(const cocos2d::Vec2& position);  // 设置卡牌位置
	bool isFlipped() const;             // 判断卡牌是否翻开
	void setFlipped(bool flipped);      // 设置卡牌翻开状态
	bool isClickable() const;           // 判断卡牌是否可点击
	void setClickable(bool clickable);  // 设置卡牌可点击状态
	int getCardValue() const;           // 获取卡牌数值（用于匹配判断）

private:
	int _cardId;
	CardSuitType _cardSuit;		//卡牌花色
	CardFaceType _cardFace;		//卡牌点数
	cocos2d::Vec2 _position;		//卡牌位置
	bool _isFlipped;	//判断卡牌是否翻开
	bool _isClickable;	//判断卡牌是否可以点击
};

#endif