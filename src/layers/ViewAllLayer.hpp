#pragma once
class ViewAllLayer : public cocos2d::CCLayer {
protected:
    cocos2d::CCMenu* m_buttonMenu;
    cocos2d::CCLabelBMFont* m_pointLabel;

    bool init() override;
    void keyBackClicked() override {
        this->onClose();
    }
    void onEnter() override;
public:
    static ViewAllLayer* create();
    static cocos2d::CCScene* scene();
    void onClose();
};
