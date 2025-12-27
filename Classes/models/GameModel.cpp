#include "GameModel.h"
#include "services/CardMatchingService.h"

GameModel::GameModel()
	: _gameState(GAME_STATE_IDLE)
	, _score(0)
	, _moveCount(0) {
}

GameModel::~GameModel() {
	// 鏅鸿兘鎸囬拡浼氳嚜鍔ㄧ鐞嗗唴瀛橈紝涓嶉渶瑕佹墜鍔ㄩ噴鏀?
}

bool GameModel::init() {
	_gameState = GAME_STATE_IDLE;
	_score = 0;
	_moveCount = 0;
	return true;
}

const std::vector<std::shared_ptr<CardModel>>& GameModel::getPlayfieldCards() const {
	return _playfieldCards;
}

void GameModel::addPlayfieldCard(CardModel* card) {
	auto cardPtr = std::shared_ptr<CardModel>(card);
	_playfieldCards.push_back(cardPtr);
	_playfieldCardMap[card->getCardId()] = cardPtr;
	CCLOG("Playfield card added. Playfield cards count: %d", (int)_playfieldCards.size());
}

void GameModel::removePlayfieldCard(int cardId) {
	auto it = _playfieldCardMap.find(cardId);
	if (it != _playfieldCardMap.end()) {
		for (auto iter = _playfieldCards.begin(); iter != _playfieldCards.end(); ++iter) {
			if ((*iter)->getCardId() == cardId) {
				_playfieldCards.erase(iter);
				break;
			}
		}
		_playfieldCardMap.erase(it);
		CCLOG("Playfield card removed. Playfield cards count: %d", (int)_playfieldCards.size());
	}
}

CardModel* GameModel::getPlayfieldCardById(int cardId) const {
	auto it = _playfieldCardMap.find(cardId);
	return (it != _playfieldCardMap.end()) ? it->second.get() : nullptr;
}

const std::vector<std::shared_ptr<CardModel>>& GameModel::getTrayCards() const {
	return _trayCards;
}

void GameModel::addTrayCard(CardModel* card) {
	auto cardPtr = std::shared_ptr<CardModel>(card);
	_trayCards.push_back(cardPtr);
	_trayCardMap[card->getCardId()] = cardPtr;
	CCLOG("Tray card added. Tray cards count: %d", (int)_trayCards.size());
}

void GameModel::removeTrayCard(int cardId) {
	auto it = _trayCardMap.find(cardId);
	if (it != _trayCardMap.end()) {
		for (auto iter = _trayCards.begin(); iter != _trayCards.end(); ++iter) {
			if ((*iter)->getCardId() == cardId) {
				_trayCards.erase(iter);
				break;
			}
		}
		_trayCardMap.erase(it);
		CCLOG("Tray card removed. Tray cards count: %d", (int)_trayCards.size());
	}
}

CardModel* GameModel::getTrayCardById(int cardId) const {
        auto it = _trayCardMap.find(cardId);
        return (it != _trayCardMap.end()) ? it->second.get() : nullptr;
}

CardModel* GameModel::getTrayCard() const {
        if (_trayCards.empty()) {
                return nullptr;
        }
        return _trayCards.back().get();
}

const std::vector<std::shared_ptr<CardModel>>& GameModel::getStackCards() const {
	return _stackCards;
}

void GameModel::addStackCard(CardModel* card) {
	auto cardPtr = std::shared_ptr<CardModel>(card);
	_stackCards.push_back(cardPtr);
	_stackCardMap[card->getCardId()] = cardPtr;
	CCLOG("Stack card added. Stack cards count: %d", (int)_stackCards.size());
}

void GameModel::removeStackCard(int cardId) {
	auto it = _stackCardMap.find(cardId);
	if (it != _stackCardMap.end()) {
		for (auto iter = _stackCards.begin(); iter != _stackCards.end(); ++iter) {
			if ((*iter)->getCardId() == cardId) {
				_stackCards.erase(iter);
				break;
			}
		}
		_stackCardMap.erase(it);
		CCLOG("Stack card removed. Stack cards count: %d", (int)_stackCards.size());
	}
}

CardModel* GameModel::getStackCardById(int cardId) const {
	auto it = _stackCardMap.find(cardId);
	return (it != _stackCardMap.end()) ? it->second.get() : nullptr;
}

CardModel* GameModel::drawStackCard() {
	if (_stackCards.empty()) {
		CCLOG("Stack is empty, cannot draw card");
		return nullptr;
	}

	CCLOG("Drawing card from stack. Before draw: Stack cards count = %d", (int)_stackCards.size());
	
	auto cardPtr = _stackCards.back();
	_stackCards.pop_back();
	_stackCardMap.erase(cardPtr->getCardId()); // 浠巑ap涓Щ闄?

	CCLOG("After draw: Stack cards count = %d", (int)_stackCards.size());
	return cardPtr.get();
}

bool GameModel::isStackEmpty() const {
	return _stackCards.empty();
}

bool GameModel::canMatchCard(const CardModel* card) const {
	if (_trayCards.empty()) {
		return false;
	}
	return CardMatchingService::canMatchWithTray(card, _trayCards.back().get());
}

bool GameModel::matchCard(CardModel* card) {
	if (!canMatchCard(card)) {
		return false;
	}

	// 浠庢父鎴忓尯鍩熺Щ闄よ鐗?
	removePlayfieldCard(card->getCardId());

	// 鏇存柊搴曠墝锛屽皢褰撳墠鍗＄墝娣诲姞鍒板簳鐗屽爢
	auto cardPtr = std::shared_ptr<CardModel>(card);
	_trayCards.push_back(cardPtr);
	_trayCardMap[card->getCardId()] = cardPtr;

	// 澧炲姞鍒嗘暟鍜屾鏁?
	_score += 10;
	incrementMoveCount();

	// 妫€鏌ユ父鎴忕姸鎬?
	if (checkWinCondition()) {
		setGameState(GAME_STATE_WIN);
	}
	else if (checkLoseCondition()) {
		setGameState(GAME_STATE_LOSE);
	}

	return true;
}

bool GameModel::replaceTrayWithStackCard() {
	CCLOG("Replacing tray with stack card. Before: Stack count = %d, Tray count = %d", 
		(int)_stackCards.size(), (int)_trayCards.size());
	
	CardModel* newTrayCard = drawStackCard();
	if (newTrayCard == nullptr) {
		CCLOG("Failed to replace tray: stack is empty");
		return false;
	}

	// 灏嗘柊搴曠墝娣诲姞鍒板簳鐗屽爢
	auto cardPtr = std::shared_ptr<CardModel>(newTrayCard);
	_trayCards.push_back(cardPtr);
	_trayCardMap[newTrayCard->getCardId()] = cardPtr;

	// 澧炲姞姝ユ暟
	incrementMoveCount();

	CCLOG("After replacing tray: Stack count = %d, Tray count = %d", 
		(int)_stackCards.size(), (int)_trayCards.size());

	// 妫€鏌ユ父鎴忕姸鎬?
	if (checkLoseCondition()) {
		setGameState(GAME_STATE_LOSE);
		CCLOG("Game state set to LOSE");
	}

	return true;
}

bool GameModel::checkWinCondition() const {
	// 娓告垙鍖哄煙涓虹┖鏃惰幏鑳?
	return _playfieldCards.empty();
}

bool GameModel::checkLoseCondition() const {
	// 娌℃湁鍙尮閰嶇殑鐗屼笖鐗屽爢涓虹┖鏃跺け璐?
	if (!_playfieldCards.empty()) {
		// 妫€鏌ユ槸鍚︽湁鍙尮閰嶇殑鐗?
		for (const auto card : _playfieldCards) {
			if (canMatchCard(card.get())) {
				return false;
			}
		}
	}

	// 妫€鏌ョ墝鍫嗘槸鍚︿负绌?
	return isStackEmpty();
}

int GameModel::getGameState() const {
	return _gameState;
}

void GameModel::setGameState(int state) {
	_gameState = state;
}

int GameModel::getScore() const {
	return _score;
}

void GameModel::setScore(int score) {
	_score = score;
}

int GameModel::getMoveCount() const {
	return _moveCount;
}

void GameModel::incrementMoveCount() {
	_moveCount++;
}

