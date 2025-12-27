#ifndef STACK_VIEW_H
#define STACK_VIEW_H

#include "cocos2d.h"
#include <functional>
#include "views/CardView.h"

class StackView : public cocos2d::Node {
public:
	static StackView* create();
	virtual bool init() override;

	// 设置栈点击回调
	void setStackClickCallback(const std::function<void()>& callback);

	// 设置底牌点击回调
	void setTrayClickCallback(const std::function<void(int)>& callback);

	// 添加备用牌堆卡片
	void addStackCard(CardView* cardView);

	// 移除备用牌堆顶部卡片
	CardView* removeTopStackCard();

	// 设置底牌卡片
	void setTrayCard(CardView* cardView);

	// 获取当前底牌卡片
	CardView* getCurrentTrayCard() const;

	// 获取备用牌堆卡片数量
	int getStackCardCount() const;

	// 播放抽取卡牌动画
	void playDrawCardAnimation();

	// 更新牌堆显示
	void updateStackDisplay();

	// 设置牌堆位置
	void setStackPosition(const cocos2d::Vec2& position);

	// 设置底牌位置
	void setTrayPosition(const cocos2d::Vec2& position);

private:
	// 栈点击回调
	std::function<void()> _stackClickCallback;

	// 底牌点击回调
	std::function<void(int)> _trayClickCallback;

	// 备用牌堆显示（只显示顶部卡片，不存储所有卡片）
	std::vector<cocos2d::Sprite*> _stackCardSprites;  // 仅用于视觉堆叠效果

	// 当前底牌卡片
	CardView* _currentTrayCard;

	// 牌堆显示节点
	cocos2d::Sprite* _stackSprite;

	// 牌堆数量显示
	cocos2d::Label* _countLabel;

	// 牌堆位置
	cocos2d::Vec2 _stackPosition;

	// 底牌位置
	cocos2d::Vec2 _trayPosition;

	// 处理触摸事件
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

	// 处理底牌点击
	bool onTrayClick(const cocos2d::Vec2& position);

	// 处理牌堆点击
	bool onStackClick(const cocos2d::Vec2& position);
};

#endif