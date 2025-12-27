#include "UndoModel.h"

USING_NS_CC;

UndoModel::UndoModel() {
    _cardId = -1;
    _fromPosition = Vec2::ZERO;
    _toPosition = Vec2::ZERO;
    _oldSuit = CardSuitType::CST_MAX;
    _oldFace = CardFaceType::CFT_MAX;
    _newSuit = CardSuitType::CST_MAX;
    _newFace = CardFaceType::CFT_MAX;
    _oldStackCardCount = 0;
    _newStackCardCount = 0;
}

UndoModel::~UndoModel() {
}

void UndoModel::setActionType(const std::string& actionType) {
    _actionType = actionType;
}

const std::string& UndoModel::getActionType() const {
    return _actionType;
}

void UndoModel::setCardId(int cardId) {
    _cardId = cardId;
}

int UndoModel::getCardId() const {
    return _cardId;
}

void UndoModel::setFromPosition(const cocos2d::Vec2& position) {
    _fromPosition = position;
}

const cocos2d::Vec2& UndoModel::getFromPosition() const {
    return _fromPosition;
}

void UndoModel::setToPosition(const cocos2d::Vec2& position) {
    _toPosition = position;
}

const cocos2d::Vec2& UndoModel::getToPosition() const {
    return _toPosition;
}

void UndoModel::setOldSuit(CardSuitType suit) {
    _oldSuit = suit;
}

CardSuitType UndoModel::getOldSuit() const {
    return _oldSuit;
}

void UndoModel::setOldFace(CardFaceType face) {
    _oldFace = face;
}

CardFaceType UndoModel::getOldFace() const {
    return _oldFace;
}

void UndoModel::setNewSuit(CardSuitType suit) {
    _newSuit = suit;
}

CardSuitType UndoModel::getNewSuit() const {
    return _newSuit;
}

void UndoModel::setNewFace(CardFaceType face) {
    _newFace = face;
}

CardFaceType UndoModel::getNewFace() const {
    return _newFace;
}

void UndoModel::setOldStackCardCount(int stackCardCount) {
    _oldStackCardCount = stackCardCount;
}

int UndoModel::getOldStackCardCount() const {
    return _oldStackCardCount;
}

void UndoModel::setNewStackCardCount(int stackCardCount) {
    _newStackCardCount = stackCardCount;
}

int UndoModel::getNewStackCardCount() const {
    return _newStackCardCount;
}


