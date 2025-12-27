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
        
        // 璁剧疆鍗＄墝鑳岄潰璧勬簮璺緞
        _cardBackRes = "res/res/card_general.png";
        CCLOG("Card back res set to: %s", _cardBackRes.c_str());
        
        // 璁剧疆绌虹墝鍫嗚祫婧愯矾寰?
        _emptyStackRes = "res/empty_stack.png";
        
        // 璁剧疆瀛椾綋璧勬簮璺緞
        _fontRes = "fonts/arial.ttf";
        CCLOG("Font res set to: %s", _fontRes.c_str());
        
        // 璁剧疆鎾ら攢鎸夐挳璧勬簮璺緞
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

                        // 璁剧疆姣忓紶姝ｉ潰鍗＄墝璧勬簮璺緞
                        // 璧勬簮鏂囦欢鍛藉悕瑙勮寖锛歜ig_{棰滆壊}_{闈㈠€紏.png
                        // 棰滆壊: red(鏂瑰潡/绾㈡), black(榛戞/姊呰姳)
                        // 闈㈠€? A, 2-10, J, Q, K
                        char resPath[128];

                        // 纭畾鍗＄墝棰滆壊
                        const char* colorPrefix = "";
                        if (suitVal == static_cast<int>(CardSuitType::CST_DIAMONDS) ||
                                suitVal == static_cast<int>(CardSuitType::CST_HEARTS)) {
                                colorPrefix = "red";
                        }
                        else {
                                colorPrefix = "black";
                        }

                        // 纭畾闈㈠€煎瓧绗︿覆
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

                        // 鏋勫缓璧勬簮璺緞
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

