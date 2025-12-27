#ifndef __CARD_MATCHING_SERVICE_H__
#define __CARD_MATCHING_SERVICE_H__

#include "models/CardModel.h"

class CardMatchingService {
public:
    // 检查两张卡牌是否匹配（点数差1）
    static bool isMatch(const CardModel* card1, const CardModel* card2);
    
    // 检查卡牌是否与底牌匹配
    static bool canMatchWithTray(const CardModel* card, const CardModel* trayCard);
};

#endif // __CARD_MATCHING_SERVICE_H__