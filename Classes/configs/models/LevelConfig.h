#ifndef __LEVEL_CONFIG_H
#define __LEVEL_CONFIG_H

#include <vector>
#include "cocos2d.h"
#include "models/CardEnums.h"

/**
 * @struct CardConfig
 * @brief 卡牌配置结构体，定义了卡牌的基本属性和初始位置
 */
struct CardConfig {
	CardFaceType cardFace;   ///< 卡牌的面值类型
	CardSuitType cardSuit;   ///< 卡牌的花色类型
	cocos2d::Vec2 position;  ///< 卡牌的初始位置
};

/**
 * @class LevelConfig
 * @brief 关卡配置类，存储单个关卡的所有配置信息
 * @details 包含关卡ID、主牌区卡牌配置和堆叠区卡牌配置
 */
class LevelConfig {
public:
	/**
	 * @brief 构造函数
	 */
	LevelConfig();
	
	/**
	 * @brief 析构函数
	 */
	virtual ~LevelConfig();

	/**
	 * @brief 获取主牌区卡牌配置列表
	 * @return 主牌区卡牌配置的常量引用
	 */
	const std::vector<CardConfig>& getPlayfieldCards() const;
	
	/**
	 * @brief 获取堆叠区卡牌配置列表
	 * @return 堆叠区卡牌配置的常量引用
	 */
	const std::vector<CardConfig>& getStackCards() const;

	/**
	 * @brief 向主牌区添加卡牌配置
	 * @param cardConfig 要添加的卡牌配置
	 */
	void addPlayfieldCard(const CardConfig& cardConfig);
	
	/**
	 * @brief 向堆叠区添加卡牌配置
	 * @param cardConfig 要添加的卡牌配置
	 */
	void addStackCard(const CardConfig& cardConfig);

	/**
	 * @brief 获取关卡ID
	 * @return 关卡ID
	 */
	int getLevelId() const;
	
	/**
	 * @brief 设置关卡ID
	 * @param levelId 要设置的关卡ID
	 */
	void setLevelId(int levelId);

private:
	int _levelId;                    ///< 关卡ID
	std::vector<CardConfig> _playfieldCards; ///< 主牌区卡牌配置列表
	std::vector<CardConfig> _stackCards;     ///< 堆叠区卡牌配置列表
};

#endif


