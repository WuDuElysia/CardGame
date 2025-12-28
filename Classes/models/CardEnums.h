#ifndef CARD_ENUMS_H
#define CARD_ENUMS_H

/**
 * @file CardEnums.h
 * @brief 卡牌枚举定义
 * @details 定义了游戏中使用的卡牌花色和点数枚举类型
 */

/**
 * @enum CardSuitType
 * @brief 卡牌花色枚举
 * @details 定义了卡牌的四种花色，从方块到黑桃，以及一个最大值标记
 */
enum class CardSuitType {
	CST_DIAMONDS = 0,  ///< 方块花色
	CST_CLUBS,         ///< 梅花花色
	CST_HEARTS,        ///< 红桃花色
	CST_SPADES,        ///< 黑桃花色
	CST_MAX            ///< 花色枚举最大值，用于循环或边界检查
};

/**
 * @enum CardFaceType
 * @brief 卡牌点数枚举
 * @details 定义了卡牌的13种点数，从A到K，以及一个最大值标记
 */
enum class CardFaceType {
	CFT_ACE = 1,   ///< A点
	CFT_TWO,       ///< 2点
	CFT_THREE,     ///< 3点
	CFT_FOUR,      ///< 4点
	CFT_FIVE,      ///< 5点
	CFT_SIX,       ///< 6点
	CFT_SEVEN,     ///< 7点
	CFT_EIGHT,     ///< 8点
	CFT_NINE,      ///< 9点
	CFT_TEN,       ///< 10点
	CFT_JACK,      ///< J点
	CFT_QUEEN,     ///< Q点
	CFT_KING,      ///< K点
	CFT_MAX        ///< 点数枚举最大值，用于循环或边界检查
};

#endif
