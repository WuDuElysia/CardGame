#ifndef __LEVEL_CONFIG_LOADER_H
#define __LEVEL_CONFIG_LOADER_H

#include "configs/models/LevelConfig.h"
#include <memory>

class LevelConfigLoader {
public:
	static std::shared_ptr<LevelConfig> loadLevelConfig(int levelId);

private:
	static std::string getLevelConfigFilePath(int levelId);
};

#endif