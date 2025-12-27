#ifndef __CARDENUMS_H
#define __CARDENUMS_H



// 卡牌花色枚举
enum class CardSuitType {
	CST_DIAMONDS = 0,  // 方块
	CST_CLUBS,         // 梅花
	CST_HEARTS,        // 红桃
	CST_SPADES,        // 黑桃
	CST_MAX
};

// 卡牌点数枚举
enum class CardFaceType {
	CFT_ACE = 1,   // A
	CFT_TWO,       // 2
	CFT_THREE,     // 3
	CFT_FOUR,      // 4
	CFT_FIVE,      // 5
	CFT_SIX,       // 6
	CFT_SEVEN,     // 7
	CFT_EIGHT,     // 8
	CFT_NINE,      // 9
	CFT_TEN,       // 10
	CFT_JACK,      // J
	CFT_QUEEN,     // Q
	CFT_KING,      // K
	CFT_MAX
};

#endif