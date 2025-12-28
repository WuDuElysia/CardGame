#include "CardMatchingService.h"

bool CardMatchingService::isMatch(const CardModel* card1, const CardModel* card2) 
{
	if (card1 == nullptr || card2 == nullptr) 
	{
		return false;
	}
	
	int value1 = card1->getCardValue();
	int value2 = card2->getCardValue();
	
	return (abs(value1 - value2) == 1); 
}

bool CardMatchingService::canMatchWithTray(const CardModel* card, const CardModel* trayCard) 
{
	// 如果底牌为空，任何卡牌都可以匹配
	if (trayCard == nullptr) 
	{
		return true;
	}
	
	return isMatch(card, trayCard); 
} 

