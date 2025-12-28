#ifndef CARD_MODEL_H
#define CARD_MODEL_H

#include "CardEnums.h"
#include "cocos2d.h"

/**
 * @class CardModel
 * @brief 卡牌数据模型类
 * @details 存储卡牌的基本信息和状态，包括卡牌ID、花色、点数、位置、翻开状态和可点击状态
 */
class CardModel {
public:
	/**
	 * @brief 默认构造函数
	 */
	CardModel();
	
	/**
	 * @brief 带参数的构造函数
	 * @param cardId 卡牌ID
	 * @param face 卡牌点数
	 * @param suit 卡牌花色
	 * @param position 卡牌位置
	 */
	CardModel(int cardId, CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position);
	
	/**
	 * @brief 显式拷贝构造函数
	 * @details 确保face和suit值被正确复制
	 * @param other 要拷贝的卡牌模型
	 */
	CardModel(const CardModel& other);
	
	/**
	 * @brief 析构函数
	 */
	virtual ~CardModel();

	/**
	 * @brief 获取卡牌ID
	 * @return 卡牌ID
	 */
	int getCardId() const;
	
	/**
	 * @brief 获取卡牌花色
	 * @return 卡牌花色枚举值
	 */
	CardSuitType getCardSuit() const;
	
	/**
	 * @brief 获取卡牌点数
	 * @return 卡牌点数枚举值
	 */
	CardFaceType getCardFace() const;
	
	/**
	 * @brief 获取卡牌位置
	 * @return 卡牌位置的常量引用
	 */
	const cocos2d::Vec2& getPosition() const;
	
	/**
	 * @brief 设置卡牌位置
	 * @param position 要设置的卡牌位置
	 */
	void setPosition(const cocos2d::Vec2& position);
	
	/**
	 * @brief 判断卡牌是否翻开
	 * @return 卡牌是否翻开
	 */
	bool isFlipped() const;
	
	/**
	 * @brief 设置卡牌翻开状态
	 * @param flipped 是否翻开
	 */
	void setFlipped(bool flipped);
	
	/**
	 * @brief 判断卡牌是否可点击
	 * @return 卡牌是否可点击
	 */
	bool isClickable() const;
	
	/**
	 * @brief 设置卡牌可点击状态
	 * @param clickable 是否可点击
	 */
	void setClickable(bool clickable);
	
	/**
	 * @brief 获取卡牌数值
	 * @details 用于匹配判断，将卡牌点数转换为对应的数值
	 * @return 卡牌数值
	 */
	int getCardValue() const;

private:
	int _cardId;                   ///< 卡牌唯一标识
	CardSuitType _cardSuit;        ///< 卡牌花色
	CardFaceType _cardFace;        ///< 卡牌点数
	cocos2d::Vec2 _position;       ///< 卡牌在游戏中的位置
	bool _isFlipped = true;        ///< 卡牌是否翻开，默认翻开
	bool _isClickable;             ///< 卡牌是否可点击
};

#endif

