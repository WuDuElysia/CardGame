#include "LevelConfigLoader.h"
#include "cocos2d.h"
#include "json/document.h"

USING_NS_CC;

std::shared_ptr<LevelConfig> LevelConfigLoader::loadLevelConfig(int levelId) {
        std::string filePath = getLevelConfigFilePath(levelId);

        // 读取配置文件
        std::string jsonContent = FileUtils::getInstance()->getStringFromFile(filePath);
        if (jsonContent.empty()) {
                CCLOG("Failed to load level config file: %s", filePath.c_str());
                return nullptr;
        }

        // 解析JSON
        rapidjson::Document document;
        document.Parse(jsonContent.c_str());

        if (!document.IsObject()) {
                CCLOG("Invalid JSON format in level config file: %s", filePath.c_str());
                return nullptr;
        }

        std::shared_ptr<LevelConfig> levelConfig = std::make_shared<LevelConfig>();
        levelConfig->setLevelId(levelId);

        // 解析游戏区域
        if (document.HasMember("Playfield") && document["Playfield"].IsArray()) {
                const rapidjson::Value& playfieldArray = document["Playfield"];
                for (rapidjson::SizeType i = 0; i < playfieldArray.Size(); ++i) {
                        const rapidjson::Value& cardObj = playfieldArray[i];
                        if (cardObj.IsObject()) {
                                CardConfig cardConfig;
                                cardConfig.cardFace = static_cast<CardFaceType>(cardObj["CardFace"].GetInt());
                                cardConfig.cardSuit = static_cast<CardSuitType>(cardObj["CardSuit"].GetInt());

                                const rapidjson::Value& positionObj = cardObj["Position"];
                                if (positionObj.IsObject()) {
                                        cardConfig.position.x = positionObj["x"].GetDouble();
                                        cardConfig.position.y = positionObj["y"].GetDouble();
                                }

                                levelConfig->addPlayfieldCard(cardConfig);
                        }
                }
        }

        // 解析牌堆区域
        if (document.HasMember("Stack") && document["Stack"].IsArray()) {
                const rapidjson::Value& stackArray = document["Stack"];
                for (rapidjson::SizeType i = 0; i < stackArray.Size(); ++i) {
                        const rapidjson::Value& cardObj = stackArray[i];
                        if (cardObj.IsObject()) {
                                CardConfig cardConfig;
                                cardConfig.cardFace = static_cast<CardFaceType>(cardObj["CardFace"].GetInt());
                                cardConfig.cardSuit = static_cast<CardSuitType>(cardObj["CardSuit"].GetInt());

                                // 牌堆卡牌位置由游戏管理器处理，默认为(0,0)
                                cardConfig.position = Vec2::ZERO;

                                levelConfig->addStackCard(cardConfig);
                        }
                }
        }

        return levelConfig;
}

std::string LevelConfigLoader::getLevelConfigFilePath(int levelId) {
        char filePath[64];
        sprintf(filePath, "res/levels/level_%d.json", levelId);
        return filePath;
}