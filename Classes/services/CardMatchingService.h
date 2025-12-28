#ifndef __CARD_MATCHING_SERVICE_H
#define __CARD_MATCHING_SERVICE_H

#include "models/CardModel.h"

/**
 * @class CardMatchingService
 * @brief 卡牌匹配服务类
 * @details 提供卡牌匹配的静态方法，用于判断两张卡牌是否可以匹配，
 *          以及卡牌是否可以与当前底牌匹配
 */
class CardMatchingService {
public:
	/**
	 * @brief 检查两张卡牌是否匹配
	 * @details 匹配规则：两张卡牌的点数差为1
	 * @param card1 第一张卡牌指针
	 * @param card2 第二张卡牌指针
	 * @return 是否匹配成功
	 */
	static bool isMatch(const CardModel* card1, const CardModel* card2);

	/**
	 * @brief 检查卡牌是否可以与当前底牌匹配
	 * @details 匹配规则：卡牌点数与底牌点数差为1
	 * @param card 要检查的卡牌指针
	 * @param trayCard 当前底牌指针
	 * @return 是否可以匹配
	 */
	static bool canMatchWithTray(const CardModel* card, const CardModel* trayCard);
};

#endif


