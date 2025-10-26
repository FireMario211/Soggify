#pragma once
#include "QuestsPopup.hpp"
class QuestNode : public cocos2d::CCNode {
    protected:
        uint8_t m_currentID;
        uint8_t m_index;
        QuestsPopup* m_layer;
        void spawnParticles(int reward);
        void exitMe();
        bool init(QuestsPopup* layer, uint8_t, uint8_t, bool withEnter = false);
    public:
        static QuestNode* create(QuestsPopup* layer, uint8_t id, uint8_t index, bool withEnter = false) {
            auto ret = new QuestNode();
            if (ret && ret->init(layer, id, index, withEnter)) {
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }
};
