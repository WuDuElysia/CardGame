#ifndef CARD_CONTROLLER_H
#define CARD_CONTROLLER_H

#include "models/CardModel.h"
#include "views/CardView.h"

class CardController {
public:
	CardController(CardModel* cardModel, CardView* cardView);
	virtual ~CardController();

	CardModel* getCardModel() const;
	CardView* getCardView() const;

private:
	std::shared_ptr<CardModel> _cardModel;
	std::shared_ptr<CardView> _cardView;
};

#endif