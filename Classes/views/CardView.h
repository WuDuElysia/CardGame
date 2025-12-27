#ifndef CARD_VIEW_H
#define CARD_VIEW_H

#include "cocos2d.h"
#include "models/CardModel.h"

class CardView : public cocos2d::Sprite {
public:
	static CardView* create(CardModel* cardModel);

	virtual bool init(CardModel* cardModel);
	CardModel* getCardModel() const;
	void setCardModel(CardModel* cardModel);

	void flipCard();  // 翻转卡牌
	bool isFlipped() const;
	void setFlipped(bool flipped);  // 设置卡牌翻转状态

private:
	CardModel* _cardModel;
	bool _isFlipped;
	
	// 卡牌组件
	cocos2d::Sprite* _backgroundSprite;
	cocos2d::Sprite* _numberSprite;
	cocos2d::Sprite* _suitSprite;
	
	// 更新卡牌显示
	void updateCardDisplay();

};

#endif