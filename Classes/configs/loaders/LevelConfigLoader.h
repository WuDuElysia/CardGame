#ifndef __LEVEL_CONFIG_LOADER_H
#define __LEVEL_CONFIG_LOADER_H

#include "configs/models/LevelConfig.h"
#include <memory>

/**
 * @class LevelConfigLoader
 * @brief 关卡配置加载器，负责从文件加载指定关卡的配置数据
 * @details 该类提供静态方法来加载和解析关卡配置文件，
 *          用于初始化游戏关卡的各项参数
 */
class LevelConfigLoader {
public:
	/**
	 * @brief 加载指定关卡ID的配置
	 * @param levelId 关卡ID
	 * @return 关卡配置对象的智能指针
	 */
	static std::shared_ptr<LevelConfig> loadLevelConfig(int levelId);

private:
	/**
	 * @brief 获取关卡配置文件的路径
	 * @param levelId 关卡ID
	 * @return 配置文件的完整路径
	 */
	static std::string getLevelConfigFilePath(int levelId);
};

#endif
