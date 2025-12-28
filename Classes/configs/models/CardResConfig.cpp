#include "CardResConfig.h"
#include "cocos2d.h"

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
        CCLOG("CardResConfig::init() called");
        
        // 设置卡牌背面资源路径
        _cardBackRes = "res/res/card_general.png";
        CCLOG("Card back res set to: %s", _cardBackRes.c_str());
        
        // 设置空牌堆资源路径
        _emptyStackRes = "res/empty_stack.png";
        
        // 设置字体资源路径
        _fontRes = "fonts/arial.ttf";
        CCLOG("Font res set to: %s", _fontRes.c_str());
        
        // 设置撤销按钮资源路径
        _undoButtonRes = "CloseNormal.png";

        for (int suitVal = static_cast<int>(CardSuitType::CST_DIAMONDS);
                suitVal <= static_cast<int>(CardSuitType::CST_SPADES);
                ++suitVal) {

                for (int faceVal = static_cast<int>(CardFaceType::CFT_ACE);
                        faceVal <= static_cast<int>(CardFaceType::CFT_KING);
                        ++faceVal) {

                        CardFaceType face = static_cast<CardFaceType>(faceVal);
                        CardSuitType suit = static_cast<CardSuitType>(suitVal);
                        int resId = generateCardResId(face, suit);

                        // 设置每种正面卡牌资源路径
                        // 资源文件命名规则: big_{颜色}_{面值}.png
                        // 颜色: red(方块/红桃), black(黑桃/梅花)
                        // 面值: A, 2-10, J, Q, K
                        char resPath[128];

                        // 确定卡牌颜色
                        const char* colorPrefix = "";
                        if (suitVal == static_cast<int>(CardSuitType::CST_DIAMONDS) ||
                                suitVal == static_cast<int>(CardSuitType::CST_HEARTS)) {
                                colorPrefix = "red";
                        }
                        else {
                                colorPrefix = "black";
                        }

                        // 确定面值字符串
                        const char* faceStr = "";
                        switch (faceVal) {
                        case 1:  faceStr = "A"; break;
                        case 11: faceStr = "J"; break;
                        case 12: faceStr = "Q"; break;
                        case 13: faceStr = "K"; break;
                        default: {
                                static char numStr[3];
                                sprintf(numStr, "%d", faceVal);
                                faceStr = numStr;
                        } break;
                        }

                        // 构建资源路径
                        sprintf(resPath, "res/res/number/big_%s_%s.png", colorPrefix, faceStr);
                        _cardFrontResMap[resId] = resPath;
                }
        }

        return true;
}

std::string CardResConfig::getCardFrontRes(CardFaceType face, CardSuitType suit) const {
        // 修复CardFace=0的情况，默认使用ACE
        CardFaceType adjustedFace = (face == static_cast<CardFaceType>(0)) ? CardFaceType::CFT_ACE : face;
        int resId = generateCardResId(adjustedFace, suit);
        auto it = _cardFrontResMap.find(resId);
        if (it != _cardFrontResMap.end()) {
                return it->second;
        }
        CCLOG("Failed to get card front res for face=%d, suit=%d", static_cast<int>(face), static_cast<int>(suit));
        return _cardBackRes; // 如果找不到，返回背面纹理
}

std::string CardResConfig::getCardBackRes() const {
        return _cardBackRes;
}

std::string CardResConfig::getEmptyStackRes() const {
        return _emptyStackRes;
}

std::string CardResConfig::getFontRes() const {
        return _fontRes;
}

std::string CardResConfig::getUndoButtonRes() const {
        return _undoButtonRes;
}

int CardResConfig::generateCardResId(CardFaceType face, CardSuitType suit) const {
        return static_cast<int>(suit) * 100 + static_cast<int>(face);
}


