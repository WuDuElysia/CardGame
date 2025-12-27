#ifndef __LEVEL_CONFIG_H
#define __LEVEL_CONFIG_H

#include <vector>
#include "cocos2d.h"
#include "models/CardEnums.h"

struct CardConfig {
	CardFaceType cardFace;
	CardSuitType cardSuit;
	cocos2d::Vec2 position;
};

class LevelConfig {
public:
	LevelConfig();
	virtual ~LevelConfig();

	const std::vector<CardConfig>& getPlayfieldCards() const;
	const std::vector<CardConfig>& getStackCards() const;

	void addPlayfieldCard(const CardConfig& cardConfig);
	void addStackCard(const CardConfig& cardConfig);

	int getLevelId() const;
	void setLevelId(int levelId);

private:
	int _levelId;
	std::vector<CardConfig> _playfieldCards;
	std::vector<CardConfig> _stackCards;
};

#endif