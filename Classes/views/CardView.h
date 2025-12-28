#ifndef CARD_VIEW_H
#define CARD_VIEW_H

#include "cocos2d.h"
#include "models/CardModel.h"

/**
 * @class CardView
 * @brief 卡牌视图类
 * @details 负责显示单个卡牌的外观，包括卡牌的花色、点数和翻转状态
 *          处理卡牌的翻转动画和显示更新
 */
class CardView : public cocos2d::Sprite {
public:
	/**
	 * @brief 创建CardView实例
	 * @param cardModel 卡牌数据模型指针
	 * @return CardView指针
	 */
	static CardView* create(CardModel* cardModel);

	/**
	 * @brief 初始化卡牌视图
	 * @param cardModel 卡牌数据模型指针
	 * @return 是否初始化成功
	 */
	virtual bool init(CardModel* cardModel);

	/**
	 * @brief 获取卡牌数据模型
	 * @return 卡牌数据模型指针
	 */
	CardModel* getCardModel() const;

	/**
	 * @brief 设置卡牌数据模型
	 * @param cardModel 要设置的卡牌数据模型指针
	 */
	void setCardModel(CardModel* cardModel);

	/**
	 * @brief 翻转卡牌
	 * @details 切换卡牌的正反面显示
	 */
	void flipCard();

	/**
	 * @brief 判断卡牌是否已翻转
	 * @return 卡牌是否已翻转
	 */
	bool isFlipped() const;

	/**
	 * @brief 设置卡牌翻转状态
	 * @param flipped 要设置的翻转状态
	 */
	void setFlipped(bool flipped);

	/**
	 * @brief 执行卡牌移动动画
	 * @param targetPos 目标位置
	 * @param duration 动画持续时间（秒）
	 * @param callback 动画完成后的回调函数
	 */
	void moveCardToPosition(const cocos2d::Vec2& targetPos, float duration,
		const std::function<void()>& callback = nullptr);

private:
	/**
	 * @brief 更新卡牌显示
	 * @details 根据卡牌模型的数据更新卡牌的外观
	 */
	void updateCardDisplay();

	CardModel* _cardModel;         ///< 卡牌数据模型指针
	bool _isFlipped;              ///< 卡牌是否已翻转

	// 卡牌组件
	cocos2d::Sprite* _backgroundSprite;  ///< 卡牌背景精灵
	cocos2d::Sprite* _numberSprite;      ///< 卡牌数字精灵
	cocos2d::Sprite* _suitSprite;        ///< 卡牌花色精灵
};

#endif
