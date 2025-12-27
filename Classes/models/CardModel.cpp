#include "CardModel.h"

CardModel::CardModel()
	: _cardId(0)
	, _cardSuit(CardSuitType::CST_DIAMONDS)
	, _cardFace(CardFaceType::CFT_ACE)
	, _position(cocos2d::Vec2::ZERO)
	, _isFlipped(false)
	, _isClickable(true) {
}

CardModel::CardModel(int cardId, CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position)
	: _cardId(cardId)
	, _cardSuit(suit)
	, _cardFace(face)
	, _position(position)
	, _isFlipped(false)
	, _isClickable(true) {
}

CardModel::~CardModel()
{
}

int CardModel::getCardId() const {
	return _cardId;
}

CardSuitType CardModel::getCardSuit() const {
	return _cardSuit;
}

CardFaceType CardModel::getCardFace() const {
	return _cardFace;
}

const cocos2d::Vec2& CardModel::getPosition() const {
	return _position;
}

void CardModel::setPosition(const cocos2d::Vec2& position) {
	_position = position;
}

bool CardModel::isFlipped() const {
	return _isFlipped;
}

void CardModel::setFlipped(bool flipped) {
	_isFlipped = flipped;
}

bool CardModel::isClickable() const {
	return _isClickable;
}

void CardModel::setClickable(bool clickable) {
	_isClickable = clickable;
}

int CardModel::getCardValue() const {
	return static_cast<int>(_cardFace);
}

