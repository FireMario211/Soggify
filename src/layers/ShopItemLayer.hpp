#pragma once
#include <defs/utils.hpp>
class ShopItemLayer : public cocos2d::CCLayer {
protected:
    cocos2d::CCMenu* m_buttonMenu;
    cocos2d::CCLabelBMFont* m_pointLabel;

    bool init(ShopItem item);
    void keyBackClicked() override {
        this->onClose();
    }
    void onEnter() override;
public:
    static ShopItemLayer* create(ShopItem item);
    static cocos2d::CCScene* scene(ShopItem item);
    void onClose();
};
