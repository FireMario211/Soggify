#pragma once

class ShopLayer : public cocos2d::CCLayer {
protected:
    cocos2d::CCMenu* m_buttonMenu;
    cocos2d::CCMenu* m_fourTitlesMenu;
    cocos2d::CCLabelBMFont* m_title;
    cocos2d::CCLabelBMFont* m_pointLabel;
    uint8_t m_currentPage = 0;
    bool m_playedLoop = false;

    cocos2d::CCPoint m_lastMousePos;

    void updateTitles();
    bool init() override;
    void keyBackClicked() override {
        this->onClose();
    }
    void onEnter() override;
    void update(float dt) override;
public:
    void sogDance(float);
    void startLoop2(float);
    static ShopLayer* create();
    static cocos2d::CCScene* scene();
    void onClose();
};
