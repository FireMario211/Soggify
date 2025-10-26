#pragma once
class QuestsPopup : public geode::Popup<>, public CurrencyRewardDelegate {
    protected:
        bool setup() override;
    public:
        CurrencyRewardLayer* m_currencyRewardLayer;
        virtual void onClose(cocos2d::CCObject* sender) override;
        virtual void currencyWillExit(CurrencyRewardLayer* param_1) override {
            if ((this->m_currencyRewardLayer == param_1) && (this->m_currencyRewardLayer != nullptr)) {
                this->m_currencyRewardLayer = nullptr;
            }
        }
        void regenNode(uint8_t index);
        static QuestsPopup* create() {
            auto ret = new QuestsPopup();
            if (ret->initAnchored(420, 280)) {
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }
};
