#pragma once
class NukeLayer : public cocos2d::CCLayer {
    protected:
        cocos2d::CCSprite* m_sog;
        bool init() override;
        void onEnterTransitionDidFinish() override;
    public:
        void playSound();
        void endScene();
        static NukeLayer* create();
        static cocos2d::CCScene* scene();
};
