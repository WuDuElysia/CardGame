#ifndef PLAY_FIELD_VIEW_H
#define PLAY_FIELD_VIEW_H

#include "cocos2d.h"
#include "views/CardView.h"

/**
 * @class PlayFieldView
 * @brief 主牌区视图类
 * @details 负责显示主牌区的卡牌，处理卡牌的添加、移除、动画效果和用户交互
 */
class PlayFieldView : public cocos2d::Node {
public:
	/**
	 * @brief 卡牌点击回调类型定义
	 * @param int 被点击卡牌的ID
	 */
	typedef std::function<void(int)> OnCardClickCallback;

	/**
	 * @brief 创建PlayFieldView实例
	 * @return PlayFieldView指针
	 */
	static PlayFieldView* create();

	/**
	 * @brief 初始化视图
	 * @return 是否初始化成功
	 */
	virtual bool init() override;

	// 卡牌管理
	/**
	 * @brief 添加卡牌视图
	 * @param cardView 要添加的卡牌视图指针
	 */
	void addCardView(CardView* cardView);

	/**
	 * @brief 移除卡牌视图
	 * @param cardView 要移除的卡牌视图指针
	 */
	void removeCardView(CardView* cardView);

	/**
	 * @brief 根据卡牌ID移除卡牌视图
	 * @param cardId 要移除的卡牌ID
	 */
	void removeCardViewByCardId(int cardId);

	/**
	 * @brief 根据卡牌ID获取卡牌视图
	 * @param cardId 卡牌ID
	 * @return 卡牌视图指针，若未找到则返回nullptr
	 */
	CardView* getCardViewByCardId(int cardId) const;

	/**
	 * @brief 清空所有卡牌视图
	 * @details 移除所有卡牌视图并清空内部列表
	 */
	void clearAllCardViews();

	// 卡牌动画
	/**
	 * @brief 将卡牌移动到指定位置
	 * @param cardView 要移动的卡牌视图
	 * @param position 目标位置
	 * @param duration 动画持续时间，默认0.3秒
	 */
	void moveCardToPosition(CardView* cardView, const cocos2d::Vec2& position, float duration = 0.3f);

	/**
	 * @brief 翻转卡牌
	 * @param cardView 要翻转的卡牌视图
	 * @param duration 动画持续时间，默认0.5秒
	 */
	void flipCard(CardView* cardView, float duration = 0.5f);

	/**
	 * @brief 缩放卡牌
	 * @param cardView 要缩放的卡牌视图
	 * @param scale 目标缩放比例
	 * @param duration 动画持续时间，默认0.2秒
	 */
	void scaleCard(CardView* cardView, const cocos2d::Vec2& scale, float duration = 0.2f);

	/**
	 * @brief 高亮显示卡牌
	 * @param cardView 要高亮的卡牌视图
	 * @param highlight 是否高亮
	 * @param duration 动画持续时间，默认0.2秒
	 */
	void highlightCard(CardView* cardView, bool highlight, float duration = 0.2f);

	// 堆叠显示设置
	/**
	 * @brief 设置卡牌堆叠位置和偏移量
	 * @param basePosition 堆叠基础位置
	 * @param offset 卡牌之间的偏移量，默认20.0f
	 */
	void setCardStackPosition(const cocos2d::Vec2& basePosition, float offset = 20.0f);

	/**
	 * @brief 更新堆叠显示
	 * @details 根据当前卡牌数量和位置信息更新卡牌的显示
	 */
	void updateStackDisplay();

	/**
	 * @brief 设置顶部卡牌是否可见
	 * @param visible 是否可见
	 */
	void setTopCardVisible(bool visible);

	// 空状态显示
	/**
	 * @brief 设置空状态显示
	 * @param visible 是否显示空状态
	 */
	void setEmptyStateVisible(bool visible);

	// 回调设置
	/**
	 * @brief 设置卡牌点击回调
	 * @param callback 卡牌点击回调函数
	 */
	void setCardClickCallback(const OnCardClickCallback& callback);

	/**
	 * @brief 执行卡牌移动到目标位置的动画
	 * @param cardView 要移动的卡牌视图
	 * @param targetPos 目标位置
	 * @param duration 动画持续时间（秒）
	 * @param callback 动画完成后的回调函数
	 */
	void moveCardToTargetWithAnimation(CardView* cardView, const cocos2d::Vec2& targetPos, 
									  float duration = 0.5f, const std::function<void()>& callback = nullptr);

private:
	// 事件处理
	/**
	 * @brief 处理鼠标按下事件
	 * @param event 鼠标事件对象
	 * @return 是否处理了事件
	 */
	bool onMouseDown(cocos2d::Event* event);

	/**
	 * @brief 处理触摸开始事件
	 * @param touch 触摸对象
	 * @param event 触摸事件对象
	 * @return 是否处理了事件
	 */
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

	/**
	 * @brief 处理触摸结束事件
	 * @param touch 触摸对象
	 * @param event 触摸事件对象
	 */
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	/**
	 * @brief 根据位置查找目标卡牌
	 * @param pos 要查找的位置
	 * @return 找到的卡牌视图指针，若未找到则返回nullptr
	 */
	CardView* findCardAtPosition(const cocos2d::Vec2& pos) const;

	// 成员变量
	std::vector<CardView*> _cardViews;              ///< 卡牌视图列表
	OnCardClickCallback _onCardClickCallback;       ///< 卡牌点击回调函数

	// 堆叠显示相关
	cocos2d::Vec2 _stackBasePosition;               ///< 堆叠基础位置
	float _stackOffset;                             ///< 卡牌之间的偏移量
	bool _topCardVisible;                           ///< 顶部卡牌是否可见

	// 空状态显示
	cocos2d::Sprite* _emptyStateSprite;             ///< 空状态精灵
	cocos2d::Label* _emptyStateLabel;               ///< 空状态标签
};

#endif
