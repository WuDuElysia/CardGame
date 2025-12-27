#include "CardController.h"

CardController::CardController(CardModel* cardModel, CardView* cardView)
	: _cardModel(std::shared_ptr<CardModel>(cardModel))
	, _cardView(std::shared_ptr<CardView>(cardView)) {
}

CardController::~CardController() {
}

CardModel* CardController::getCardModel() const {
	return _cardModel.get();
}

CardView* CardController::getCardView() const {
	return _cardView.get();
}

