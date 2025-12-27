#include "LevelConfig.h"

LevelConfig::LevelConfig() : _levelId(0) {
}

LevelConfig::~LevelConfig() {
}

const std::vector<CardConfig>& LevelConfig::getPlayfieldCards() const {
	return _playfieldCards;
}

const std::vector<CardConfig>& LevelConfig::getStackCards() const {
	return _stackCards;
}

void LevelConfig::addPlayfieldCard(const CardConfig& cardConfig) {
	_playfieldCards.push_back(cardConfig);
}

void LevelConfig::addStackCard(const CardConfig& cardConfig) {
	_stackCards.push_back(cardConfig);
}

int LevelConfig::getLevelId() const {
	return _levelId;
}

void LevelConfig::setLevelId(int levelId) {
	_levelId = levelId;
}

