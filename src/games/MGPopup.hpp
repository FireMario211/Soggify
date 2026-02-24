#pragma once
class MGPopup : public geode::Popup {
    protected:
        cocos2d::CCMenu* m_gameMenu;
        bool init() override;
    public:
        virtual void onClose(cocos2d::CCObject* sender) override;
        static MGPopup* create() {
            auto ret = new MGPopup();
            if (ret->init()) {
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }
};
