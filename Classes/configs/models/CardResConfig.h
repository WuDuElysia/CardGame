#ifndef CARD_RES_CONFIG_H
#define CARD_RES_CONFIG_H

#include <string>
#include <unordered_map>
#include "models/CardEnums.h"

/**
 * @class CardResConfig
 * @brief 卡牌资源配置类，单例模式，负责管理游戏中所有卡牌相关资源的路径
 * @details 提供访问卡牌正面、背面、空堆叠、字体和撤销按钮等资源的方法
 */
class CardResConfig {
public:
	/**
	 * @brief 获取单例实例
	 * @return 返回CardResConfig的唯一实例
	 */
	static CardResConfig* getInstance();
	
	/**
	 * @brief 初始化资源配置
	 * @return 初始化是否成功
	 */
	bool init();
	
	/**
	 * @brief 获取指定花色和面值的卡牌正面资源路径
	 * @param face 卡牌面值类型
	 * @param suit 卡牌花色类型
	 * @return 卡牌正面资源的文件路径
	 */
	std::string getCardFrontRes(CardFaceType face, CardSuitType suit) const;
	
	/**
	 * @brief 获取卡牌背面资源路径
	 * @return 卡牌背面资源的文件路径
	 */
	std::string getCardBackRes() const;
	
	/**
	 * @brief 获取空堆叠资源路径
	 * @return 空堆叠资源的文件路径
	 */
	std::string getEmptyStackRes() const;
	
	/**
	 * @brief 获取字体资源路径
	 * @return 字体资源的文件路径
	 */
	std::string getFontRes() const;
	
	/**
	 * @brief 获取撤销按钮资源路径
	 * @return 撤销按钮资源的文件路径
	 */
	std::string getUndoButtonRes() const;

private:
	/**
	 * @brief 私有构造函数，实现单例模式
	 */
	CardResConfig();
	
	/**
	 * @brief 析构函数
	 */
	virtual ~CardResConfig();
	
	static CardResConfig* _instance; ///< 单例实例指针

	std::unordered_map<int, std::string> _cardFrontResMap; ///< 卡牌正面资源映射表，键为卡牌ID，值为资源路径
	std::string _cardBackRes; ///< 卡牌背面资源路径
	std::string _emptyStackRes; ///< 空堆叠资源路径
	std::string _fontRes; ///< 字体资源路径
	std::string _undoButtonRes; ///< 撤销按钮资源路径

	/**
	 * @brief 根据卡牌面值和花色生成资源ID
	 * @param face 卡牌面值类型
	 * @param suit 卡牌花色类型
	 * @return 生成的卡牌资源ID
	 */
	int generateCardResId(CardFaceType face, CardSuitType suit) const;
};

#endif
