#pragma once
class SoggifyMenuLayer : public cocos2d::CCLayer {
protected:
    cocos2d::CCLabelBMFont* m_pointLabel;
    cocos2d::CCMenu* m_buttonMenu;
    cocos2d::CCMenu* m_mainBtns;
    CCMenuItemSpriteExtra* m_funnyBtn;
    CCMenuItemSpriteExtra* m_minigamesBtn;
    CCMenuItemSpriteExtra* m_battleBtn;
    CCMenuItemSpriteExtra* m_hmbattleBtn;

    void jumpscare(float);
    bool init() override;
    void keyBackClicked() override {
        this->onClose();
    }
    void onEnter() override;
public:
    void updatePoints();
    void showPopup(float);
    static SoggifyMenuLayer* create();
    static cocos2d::CCScene* scene();
    void onClose();
};
