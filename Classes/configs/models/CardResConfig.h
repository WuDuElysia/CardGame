#ifndef __CARD_RES_CONFIG_H
#define __CARD_RES_CONFIG_H

#include <string>
#include <unordered_map>
#include "models/CardEnums.h"

class CardResConfig {
public:
	static CardResConfig* getInstance();
	bool init();
	std::string getCardFrontRes(CardFaceType face, CardSuitType suit) const;
	std::string getCardBackRes() const;

private:
	CardResConfig();
	virtual ~CardResConfig();
	static CardResConfig* _instance;

	std::unordered_map<int, std::string> _cardFrontResMap;
	std::string _cardBackRes;

	int generateCardResId(CardFaceType face, CardSuitType suit) const;
};

#endif