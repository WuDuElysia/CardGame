#ifndef STACK_VIEW_H
#define STACK_VIEW_H

#include "cocos2d.h"
#include <functional>
#include "views/CardView.h"

/**
 * @class StackView
 * @brief 牌堆视图类
 * @details 负责显示备用牌堆和底牌堆，处理牌堆的点击事件和动画效果
 *          管理牌堆卡牌的添加、移除和位置更新
 */
class StackView : public cocos2d::Node {
public:
	/**
	 * @brief 创建StackView实例
	 * @return StackView指针
	 */
	static StackView* create();

	/**
	 * @brief 初始化视图
	 * @return 是否初始化成功
	 */
	virtual bool init() override;

	/**
	 * @brief 设置牌堆点击回调
	 * @param callback 点击回调函数
	 */
	void setStackClickCallback(const std::function<void()>& callback);

	/**
	 * @brief 设置底牌点击回调
	 * @param callback 点击回调函数，参数为卡牌ID
	 */
	void setTrayClickCallback(const std::function<void(int)>& callback);

	/**
	 * @brief 添加备用牌堆卡片
	 * @param cardView 要添加的卡牌视图指针
	 */
	void addStackCard(CardView* cardView);

	/**
	 * @brief 移除备用牌堆顶部卡片
	 * @return 移除的卡牌视图指针，若牌堆为空则返回nullptr
	 */
	CardView* removeTopStackCard();

	/**
	 * @brief 设置底牌卡片
	 * @param cardView 要设置的底牌视图指针
	 */
	void setTrayCard(CardView* cardView);

	/**
	 * @brief 获取当前底牌卡片
	 * @return 当前底牌视图指针，若底牌为空则返回nullptr
	 */
	CardView* getCurrentTrayCard() const;

	/**
	 * @brief 获取备用牌堆卡片数量
	 * @return 牌堆卡片数量
	 */
	int getStackCardCount() const;

	/**
	 * @brief 播放抽取卡牌动画
	 * @details 为顶部卡牌添加动画效果
	 */
	void playDrawCardAnimation();

	/**
	 * @brief 更新牌堆显示
	 * @details 根据当前牌堆卡片数量更新显示
	 */
	void updateStackDisplay();

	/**
	 * @brief 设置牌堆位置
	 * @param position 牌堆基础位置
	 */
	void setStackPosition(const cocos2d::Vec2& position);

	/**
	 * @brief 设置底牌位置
	 * @param position 底牌位置
	 */
	void setTrayPosition(const cocos2d::Vec2& position);

	/**
	 * @brief 获取底牌位置
	 * @return 底牌位置
	 */
	cocos2d::Vec2 getTrayPosition() const;

	/**
	 * @brief 清空所有牌堆卡牌
	 * @details 用于重新初始化时清空现有卡牌
	 */
	void clearAllStackCards();

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
	/**
	 * @brief 处理触摸事件
	 * @param touch 触摸对象
	 * @param event 触摸事件对象
	 * @return 是否处理了事件
	 */
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

	/**
	 * @brief 处理底牌点击
	 * @param position 点击位置
	 * @return 是否点击了底牌
	 */
	bool onTrayClick(const cocos2d::Vec2& position);

	/**
	 * @brief 处理牌堆点击
	 * @param position 点击位置
	 * @return 是否点击了牌堆
	 */
	bool onStackClick(const cocos2d::Vec2& position);

	std::function<void()> _stackClickCallback;  ///< 牌堆点击回调函数
	std::function<void(int)> _trayClickCallback;  ///< 底牌点击回调函数

	std::vector<CardView*> _stackCardViews;  ///< 备用牌堆卡牌视图列表
	CardView* _currentTrayCard;  ///< 当前底牌视图指针

	cocos2d::Label* _countLabel;  ///< 牌堆数量显示标签

	cocos2d::Vec2 _stackPosition;  ///< 牌堆位置
	cocos2d::Vec2 _trayPosition;  ///< 底牌位置
};

#endif
