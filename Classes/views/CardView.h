#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "models/CardModel.h"

class CardView : public cocos2d::Sprite {
public:
	static std::shared_ptr<CardView> create(CardModel* cardModel);

	virtual bool init(CardModel* cardModel);
	CardModel* getCardModel() const;
	void setCardModel(CardModel* cardModel);

	void flipCard();  // 翻转卡牌
	bool isFlipped() const;


private:
	std::shared_ptr<CardModel> _cardModel;
	//CardModel* _cardModel;
	bool _isFlipped;

};

#endif