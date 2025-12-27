#ifndef CARD_RES_CONFIG_H
#define CARD_RES_CONFIG_H

#include <string>
#include <unordered_map>
#include "models/CardEnums.h"

class CardResConfig {
public:
	static CardResConfig* getInstance();
	bool init();
	std::string getCardFrontRes(CardFaceType face, CardSuitType suit) const;
	std::string getCardBackRes() const;
	std::string getEmptyStackRes() const;
	std::string getFontRes() const;
	std::string getUndoButtonRes() const;

private:
	CardResConfig();
	virtual ~CardResConfig();
	static CardResConfig* _instance;

	std::unordered_map<int, std::string> _cardFrontResMap;
	std::string _cardBackRes;
	std::string _emptyStackRes;
	std::string _fontRes;
	std::string _undoButtonRes;

	int generateCardResId(CardFaceType face, CardSuitType suit) const;
};

#endif