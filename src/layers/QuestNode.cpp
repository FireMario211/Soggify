#include "QuestNode.hpp"
#include <defs/utils.hpp>
#include <random>

// quest ideas (add a reshuffle for individual):
// find sog (hide and seek lol)
// gain X stars in this session
// beat a demon
// play and beat a mini-game
// (add some mini game conditions too idk)

class ProgressBar : public cocos2d::CCLayer {
    protected:
        cocos2d::CCSprite* m_bg;
        float m_currentProgress = 0.f;
        int increment = 1;
        cocos2d::CCSprite* m_bar;
        
        float getScaleForBar(float progress) {
            float min = 0.f;
            float max = 0.987f;
            return min + (progress / 100.f) * (max - min);
        }
        virtual bool init(float scale, float scaleX, float progress, cocos2d::ccColor3B color, std::string text) {
            if (!CCLayer::init()) return false;
            m_bg = CCSprite::create("GJ_progressBar_001.png");
            m_bg->setColor({0,0,0});
            m_bg->setOpacity(125);
            m_bg->setScale(scale);

            m_bar = CCSprite::create("GJ_progressBar_001.png");
            m_bar->setScaleY(0.825F);
            updateColor(color);
            updateProgress(progress);
            m_bg->addChildAtPosition(m_bar, Anchor::Left, {2, 0});
            m_bar->setAnchorPoint({0, 0.5});
            if (!text.empty()) {
                auto label = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
                label->setScale(m_bar->getScaledContentHeight() / label->getContentHeight());
                m_bg->addChildAtPosition(label, Anchor::Center);
                if (scaleX < 1.0f) {
                    label->setScaleX((label->getScaleX() * scaleX) + (1.0f - scaleX));
                }
            }
            this->setContentSize(m_bg->getScaledContentSize());
            this->setAnchorPoint({0.5, 0.5});
            this->addChildAtPosition(m_bg, Anchor::Center);
            if (scaleX < 1.0f) {
                this->setScaleX(scaleX);
            }
            return true;
        }
    public:
        void updateColor(cocos2d::ccColor3B color) {
            m_bar->setColor(color);
        }
        void updateProgress(float progress) {
            m_currentProgress = std::clamp(progress, 0.f, 100.f);
            m_bar->setScaleX(getScaleForBar(progress));
        }
        static ProgressBar* create(float progress = 0.f, float scale = 0.7f, cocos2d::ccColor3B color = {0,255,0}, std::string text = "", float scaleX = 1.0f) {
            auto ret = new ProgressBar();
            if (ret->init(scale, scaleX, progress, color, text)) {
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }
};

// this for whatever reason makes the center of the ccscale9 break the rect, I DONT KNOW WHY
void QuestNode::spawnParticles(int reward) {
    if (m_layer) {
        if (m_layer->m_currencyRewardLayer == nullptr) {
            m_layer->m_currencyRewardLayer = CurrencyRewardLayer::create(
                0,0,0,
                reward,
                CurrencySpriteType::Diamond,0,CurrencySpriteType::Diamond,
                0,
                this->getPosition(),CurrencyRewardType::Default,0.0,1.0
            );
            m_layer->m_currencyRewardLayer->m_diamondsLabel->setString(std::to_string(Mod::get()->getSavedValue<int>("points")).c_str());
            m_layer->m_currencyRewardLayer->m_diamonds = Mod::get()->getSavedValue<int>("points");
            m_layer->m_currencyRewardLayer->m_diamondsSprite = CCSprite::create("SOGGY.png"_spr);
            m_layer->m_currencyRewardLayer->m_diamondsSprite->setScale(0.5f);
            if (auto node = m_layer->m_currencyRewardLayer->m_mainNode->getChildByType<CCSprite*>(0)) {
                sogutils::sog(node);
            }
            m_layer->m_currencyRewardLayer->m_delegate = m_layer;
            m_layer->addChild(
                m_layer->m_currencyRewardLayer,
                14
            );
        } else {
            m_layer->m_currencyRewardLayer->createObjects(CurrencySpriteType::Diamond,reward,this->getPosition(),0);
        }
    }
}

void QuestNode::exitMe() {
    m_layer->regenNode(m_index);
    this->removeMeAndCleanup();
}

// when claiming a quest, show on the bottom of the sog points going up (gold font) and play gold coin award sfx
// https://www.youtube.com/watch?v=RG8Gpjb-KNo
bool QuestNode::init(QuestsPopup* layer, uint8_t id, uint8_t index, bool withEnter) {
    if (!CCNode::init()) return false;
    m_layer = layer;
    m_currentID = id;
    m_index = index;
    auto quests = sogutils::getQuests();
    if (m_currentID < 0 || m_currentID > quests.size() - 1) return false;
    auto quest = quests.at(m_currentID);
    auto progress = Mod::get()->getSavedValue<int>(fmt::format("q_{}", m_currentID));
    bool canClaim = progress >= quest.maxProgress;
    auto bgSpr = (canClaim) ? "GJ_square03.png" : "GJ_square02.png";
    this->setContentSize({300, 70});
    this->setAnchorPoint({0.5, 0.5});
    if (withEnter) {
        this->runAction(CCSequence::createWithTwoActions(
            CCMoveBy::create(0, {160, 0}),
            CCEaseElasticOut::create(CCMoveBy::create(0.5, {-160, 0}), 0.6f)
        ));
    }
    Build<CCScale9Sprite>::create(bgSpr)
        .contentSize(300, 70)
        .with([quest, progress, withEnter](CCScale9Sprite* node) { // because ProgressBar
            auto bar = ProgressBar::create(((float)progress / (float)quest.maxProgress) * 100.f, 0.5f, {0, 255, 0}, fmt::format("{}/{}", progress, quest.maxProgress), 1.0f);
            bar->setAnchorPoint({0, 0});
            node->addChildAtPosition(bar, Anchor::BottomLeft, {10, 12});
        })
        .children(
            Build<CCLabelBMFont>::create(quest.name.c_str(), "bigFont.fnt").scale(0.5).anchorPoint(0, 1).setAnchorOpts(geode::Anchor::TopLeft, {10, -6}), // -140, 29
            Build<CCLabelBMFont>::create(quest.requirement.c_str(), "goldFont.fnt").scale(0.5f).anchorPoint(0, 1).setAnchorOpts(geode::Anchor::Left, {10, 9}), // -140, 9
            Build<CCMenu>::create().children(
                Build<CCSprite>::createSpriteName("GJ_rewardBtn_001.png").scale(0.75f).anchorPoint(1, 0.5).intoMenuItem([this, quest, canClaim](auto node) {
                    node->setEnabled(false);
                    if (canClaim) {
                        int reward = sogutils::random_int(quest.reward[0], quest.reward[1]);
                        FMODAudioEngine::sharedEngine()->playEffect("highscoreGet02.ogg", 1, 0, GameManager::get()->m_sfxVolume);
                        //spawnParticles(reward);
                        Mod::get()->setSavedValue("points", Mod::get()->getSavedValue<int>("points", 0) + reward);
                        if (Mod::get()->hasSavedValue("quests")) {
                            auto allQuests_t = sogutils::getQuests();
                            auto allQuests = (Mod::get()->hasSavedValue("i_8") && Mod::get()->getSavedValue<bool>("i_8")) ? allQuests_t : std::vector<Quest>(allQuests_t.begin(), allQuests_t.begin() + std::min(5, static_cast<int>(allQuests_t.size())));
                            auto result = Mod::get()->getSavedValue<std::vector<int>>("quests");

                            std::vector<char> used(allQuests.size(), 0);
                            for (int id : result) {
                                used[id] = 1;
                            }
                            std::vector<int> available;
                            available.reserve(allQuests.size() - result.size());
                            for (int i = 0; i < (int)allQuests.size(); ++i) {
                                if (!used[i]) available.push_back(i);
                            }
                            std::random_device rd;
                            std::mt19937 gen(rd());
                            std::uniform_int_distribution<int> dist(0, (int)available.size() - 1);
                            int newIdx = available[dist(gen)];
                            Mod::get()->setSavedValue(fmt::format("q_{}", m_currentID), 0);
                            m_currentID = newIdx;
                            Mod::get()->setSavedValue(fmt::format("q_{}", m_currentID), 0);
                            result[m_index] = m_currentID;
                            log::debug("quests are {}", result);
                            Mod::get()->setSavedValue("quests", result);
                        }
                        this->runAction(CCSequence::createWithTwoActions(
                            CCEaseElasticIn::create(CCMoveBy::create(0.5, {-160, 0}), 1.2f),
                            CCCallFunc::create(this, callfunc_selector(QuestNode::exitMe))
                        ));
                    } else {
                        FLAlertLayer::create("what", "how did you do this", "idk")->show();
                    }
                }).visible(canClaim)
            ).setAnchorOpts(geode::Anchor::Right, {-30, 6}),
            Build<CCLabelBMFont>::create(fmt::format("{}x-{}x", quest.reward[0], quest.reward[1]).c_str(), "bigFont.fnt").scale((canClaim) ? 0.4f : 0.55f).anchorPoint(1, 0.5).setAnchorOpts(geode::Anchor::Right, (canClaim) ? CCPoint(-3, -20) : CCPoint(-10, 0))
        )
        .parentAtPos(this, Anchor::Center)
        .updateLayout();
    return true;
}
