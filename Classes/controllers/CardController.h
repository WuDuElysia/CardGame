#ifndef CARD_CONTROLLER_H
#define CARD_CONTROLLER_H

#include "models/CardModel.h"
#include "views/CardView.h"

/**
 * @class CardController
 * @brief 卡牌控制器，负责连接卡牌模型和视图
 * @details 管理单个卡牌的模型数据和视图显示，
 *          处理卡牌的交互逻辑和状态更新
 */
class CardController {
public:
	/**
	 * @brief 构造函数
	 * @param cardModel 卡牌模型指针
	 * @param cardView 卡牌视图指针
	 */
	CardController(CardModel* cardModel, CardView* cardView);
	
	/**
	 * @brief 析构函数
	 */
	virtual ~CardController();

	/**
	 * @brief 获取卡牌模型
	 * @return 卡牌模型指针
	 */
	CardModel* getCardModel() const;
	
	/**
	 * @brief 获取卡牌视图
	 * @return 卡牌视图指针
	 */
	CardView* getCardView() const;

private:
	std::shared_ptr<CardModel> _cardModel; ///< 卡牌模型智能指针
	std::shared_ptr<CardView> _cardView;   ///< 卡牌视图智能指针
};

#endif
