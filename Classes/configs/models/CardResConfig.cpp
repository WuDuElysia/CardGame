#include "CardResConfig.h"

CardResConfig* CardResConfig::_instance = nullptr;

CardResConfig::CardResConfig() {
}

CardResConfig::~CardResConfig() {
}

CardResConfig* CardResConfig::getInstance() {
        if (_instance == nullptr) {
                _instance = new CardResConfig();
                _instance->init();
        }
        return _instance;
}

bool CardResConfig::init() {
        // 设置卡牌背面资源路径
        _cardBackRes = "res/res/card_general.png";

        for (int suitVal = static_cast<int>(CardSuitType::CST_DIAMONDS);
                suitVal <= static_cast<int>(CardSuitType::CST_SPADES);
                ++suitVal) {

                for (int faceVal = static_cast<int>(CardFaceType::CFT_ACE);
                        faceVal <= static_cast<int>(CardFaceType::CFT_KING);
                        ++faceVal) {

                        CardFaceType face = static_cast<CardFaceType>(faceVal);
                        CardSuitType suit = static_cast<CardSuitType>(suitVal);
                        int resId = generateCardResId(face, suit);

                        // 设置卡牌正面资源路径
                        // 资源文件命名规范：big_{颜色}_{点数}.png
                        // 颜色: red(红桃/方块), black(黑桃/梅花)
                        // 点数: A, 2-10, J, Q, K
                        char resPath[128];

                        // 确定花色颜色
                        const char* colorPrefix = "";
                        if (suitVal == static_cast<int>(CardSuitType::CST_DIAMONDS) ||
                                suitVal == static_cast<int>(CardSuitType::CST_HEARTS)) {
                                colorPrefix = "red";
                        }
                        else {
                                colorPrefix = "black";
                        }

                        // 确定点数字符串
                        const char* faceStr = "";
                        switch (faceVal) {
                        case 1:  faceStr = "A"; break;
                        case 11: faceStr = "J"; break;
                        case 12: faceStr = "Q"; break;
                        case 13: faceStr = "K"; break;
                        default: {
                                // 将数字转换为字符串
                                static char numStr[3];
                                sprintf(numStr, "%d", faceVal);
                                faceStr = numStr;
                        } break;
                        }

                        // 生成资源路径
                        sprintf(resPath, "res/res/number/big_%s_%s.png", colorPrefix, faceStr);
                        _cardFrontResMap[resId] = resPath;
                }
        }

        return true;
}

std::string CardResConfig::getCardFrontRes(CardFaceType face, CardSuitType suit) const {
        int resId = generateCardResId(face, suit);
        auto it = _cardFrontResMap.find(resId);
        if (it != _cardFrontResMap.end()) {
                return it->second;
        }
        return "";
}

std::string CardResConfig::getCardBackRes() const {
        return _cardBackRes;
}

int CardResConfig::generateCardResId(CardFaceType face, CardSuitType suit) const {
        return static_cast<int>(suit) * 100 + static_cast<int>(face);
}