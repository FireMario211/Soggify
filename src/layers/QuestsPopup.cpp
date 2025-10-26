#include "QuestsPopup.hpp"
#include "QuestNode.hpp"
#include "SoggifyMenuLayer.hpp"
#include <defs/utils.hpp>
#include <random>

void QuestsPopup::regenNode(uint8_t i) {
    auto quests = Mod::get()->getSavedValue<std::vector<int>>("quests");
    switch (i) {
        case 0:
            m_buttonMenu->addChildAtPosition(QuestNode::create(this, quests[i], i, true), Anchor::Center, {0, 80});
            break;
        case 1:
            m_buttonMenu->addChildAtPosition(QuestNode::create(this, quests[i], i, true), Anchor::Center);
            break;
        case 2:
            m_buttonMenu->addChildAtPosition(QuestNode::create(this, quests[i], i, true), Anchor::Center, {0, -80});
            break;
    }
}

bool QuestsPopup::setup() {
    if (!Mod::get()->hasSavedValue("quests")) {
        auto allQuests_t = sogutils::getQuests();
        auto allQuests = (Mod::get()->hasSavedValue("i_8") && Mod::get()->getSavedValue<bool>("i_8")) ? allQuests_t : std::vector<Quest>(allQuests_t.begin(), allQuests_t.begin() + std::min(5, static_cast<int>(allQuests_t.size())));
        std::vector<int> pool(allQuests.size());
        for (int i = 0; i < allQuests.size(); i++) {
            pool[i] = static_cast<int>(i);
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(pool.begin(), pool.end(), gen);
        std::vector<int> result(pool.begin(), pool.begin() + 3);
        log::debug("quests are {}", result);
        Mod::get()->setSavedValue("quests", result);
    }
    if (!Mod::get()->hasSavedValue("quests")) return true;
    auto quests = Mod::get()->getSavedValue<std::vector<int>>("quests");

    for (int i = 0; i < quests.size(); i++) {
        switch (i) {
            case 0:
                m_buttonMenu->addChildAtPosition(QuestNode::create(this, quests[i], i), Anchor::Center, {0, 80});
                break;
            case 1:
                m_buttonMenu->addChildAtPosition(QuestNode::create(this, quests[i], i), Anchor::Center);
                break;
            case 2:
                m_buttonMenu->addChildAtPosition(QuestNode::create(this, quests[i], i), Anchor::Center, {0, -80});
                break;
        }
    }
    Build<CCLabelBMFont>::create("Quests will be refreshed upon restarting or completing a quest.", "goldFont.fnt").scale(0.4f).alignment(cocos2d::kCCTextAlignmentCenter).anchorPoint(0.5, 0).parentAtPos(m_buttonMenu, Anchor::Bottom, {0, 10});
    this->setID("QuestsPopup"_spr);
    return true;
}

void QuestsPopup::onClose(CCObject* sender) {
    if (m_currencyRewardLayer) {
        m_currencyRewardLayer->m_delegate = nullptr;
    }
    if (auto menu = static_cast<SoggifyMenuLayer*>(this->getParent()->getChildByID("SoggifyMenuLayer"_spr))) {
        menu->updatePoints();
    }
    Popup::onClose(sender);
}
