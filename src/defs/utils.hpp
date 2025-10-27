#pragma once
#include <string>
#include <vector>
#include "geode.hpp"

struct ShopSettings {
    uint32_t id;
    std::string name;
    std::string desc;
    int defaultValue; // if its bool then its casted... ok
    uint8_t type = 0; // 0 = bool, 1 = int
};

struct ShopItem {
    uint32_t id;
    std::string name;
    std::string desc;
    uint32_t price;
    uint32_t categoryId;
    //std::vector<ShopSettings> settings;
};

struct ShopCategory {
    uint32_t id;
    std::string name;
};

enum QuestType {
    NA,
    StarMoon,
    FindSog,
    Demon,
    MinigameS,
    MinigameP
};
struct Quest {
    std::string name;
    std::string requirement;
    uint16_t reward[2];
    uint16_t maxProgress;
    QuestType type;
};

namespace sogutils {
    inline int random_int(int min, int max) {
        return min + rand() % (max+1 - min);
    }
    inline void toggleLayerHook(std::string name, bool enable) {
        // the only reason this exists is for performance reasons, of course i dont want to always have to keep hooking it...
        for (auto& hook : geode::Mod::get()->getHooks()) {
            if (hook->getDisplayName() == name) {
                if (enable) {
                    (void)hook->enable();
                } else {
                    (void)hook->disable();
                }
            }
        }
    }
    inline void sog(cocos2d::CCNode* node, cocos2d::CCPoint anchor = {0, 0}, cocos2d::ccColor3B color = {255,255,255}) {
        if (node == nullptr) return;
        auto spr = cocos2d::CCSprite::create("SOGGY.png"_spr);
        spr->setScaleX(node->getContentWidth() / spr->getContentWidth());
        spr->setScaleY(node->getContentHeight() / spr->getContentHeight());
        spr->setColor(color);
        spr->setAnchorPoint(anchor);
        node->addChild(spr);
    }
    // 170
    inline cocos2d::extension::CCScale9Sprite* createShopButton(std::string title, cocos2d::CCSize size = {120, 40}, bool glow = true) {
        cocos2d::extension::CCScale9Sprite* bg;
        auto btn = Build<cocos2d::extension::CCScale9Sprite>::create("square02b_small.png")
            .color({60,190,230})
            .contentSize(size)
            .layout(AnchorLayout::create())
            .children(
                Build<cocos2d::extension::CCScale9Sprite>::create("square02b_small.png")
                    .store(bg)
                    .color({245,255,255})
                    .contentSize({size.width - 3, size.height - 3})
                    .setAnchorOpts(geode::Anchor::Center),
                Build<cocos2d::CCLabelBMFont>::create(title.c_str(), "PGothic.fnt"_spr)
                    .color(40, 40, 40)
                    .scale(0.4f)
                    .alignment(cocos2d::CCTextAlignment::kCCTextAlignmentCenter)
                    .setAnchorOpts(geode::Anchor::Center)
            )
            .updateLayout();

        if (glow) {
            Build<cocos2d::CCSprite>::createSpriteName("d_gradient_01_001.png")
                .color(222, 238, 243)
                .anchorPoint(0.5, 0)
                .scaleY(1.2f)
                .scaleToMatchX(bg->getContentWidth())
                .parentAtPos(bg, geode::Anchor::Bottom);

            Build<cocos2d::CCNode>::create()
                .zOrder(-1)
                .children(
                    Build<cocos2d::CCSprite>::createSpriteName("d_gradient_02_001.png").opacity(100).color(0,0,0).pos(.25,.25).anchorPoint(1,0),
                    Build<cocos2d::CCSprite>::createSpriteName("d_gradient_02_001.png").opacity(100).color(0,0,0).pos(.25,.25).scaleX(-1).scaleY(1).anchorPoint(1,0),
                    Build<cocos2d::CCSprite>::createSpriteName("d_gradient_02_001.png").opacity(100).color(0,0,0).pos(.25,.25).scaleX(1).scaleY(-1).anchorPoint(1,0),
                    Build<cocos2d::CCSprite>::createSpriteName("d_gradient_02_001.png").opacity(100).color(0,0,0).pos(.25,.25).scaleX(-1).scaleY(-1).anchorPoint(1,0))
                .scaleX(size.width / 25.f)
                .scaleY(size.height / 25.f)
                .parentAtPos(btn, geode::Anchor::Center);
        }

        return btn;
    }
    inline void addHidingSog(cocos2d::CCLayer* layer, cocos2d::CCMenu* menu) {
        if (Mod::get()->hasSavedValue("q_0") && Mod::get()->getSavedValue<int>("q_0") > 0) return;
        if (rand() % 2 == 1) {
            auto v = Mod::get()->getSavedValue<std::vector<int>>("quests");
            if (std::find(v.begin(), v.end(), 0) != v.end()) {
                // this is bad
                class tmpClass {
                    public:
                        void callback() {
                            FMODAudioEngine::sharedEngine()->playEffect("highscoreGet02.ogg", 1, 0, GameManager::get()->m_sfxVolume);
                            Mod::get()->setSavedValue("q_0", 1);
                            FLAlertLayer::create("congrats", "you found <cy>sog</c> and completed the <cy>quest</c>!\ncheck the <cy>soggify menu</c> found in the main menu to claim the <cy>quest</c>!", "OK")->show();
                        }
                };
                auto btn = CCMenuItemExt::createSpriteExtraWithFilename("SOGGY.png"_spr, 0.2f, [](CCMenuItemSpriteExtra* btn) {
                    btn->setEnabled(false);
                    btn->runAction(CCSequence::create(
                        CCEaseOut::create(CCMoveBy::create(0.25f, ccp(0, 30)), 2.0f),
                        CCSpawn::createWithTwoActions(CCEaseIn::create(CCMoveBy::create(0.25f, ccp(0, -90)), 3.0f), CCFadeOut::create(0.25f)),
                        CCCallFunc::create(nullptr, callfunc_selector(tmpClass::callback)),
                        nullptr
                    ));
                });
                switch (rand() % 4) {
                    case 0:
                        btn->setPosition(5, 5);
                        break;
                    case 1:
                        btn->setPosition(190, 5);
                        break;
                    case 2:
                        btn->setPosition(190, 190);
                        break;
                    case 3:
                        btn->setPosition(5, 190);
                        break;
                }
                if (menu != nullptr) {
                    menu->addChild(btn);
                } else {
                    if (auto men = layer->getChildByType<CCMenu*>(0)) {
                        men->addChild(btn);
                    }
                }
                log::debug("spawn secret sog");
            }
        }
    }
    inline void updateHooks(uint8_t id, bool enabled) {
        switch (id) {
            case 2: // sog icons
                sogutils::toggleLayerHook("PlayerObject::init", enabled);
                sogutils::toggleLayerHook("PlayerObject::updatePlayerFrame", enabled);
                sogutils::toggleLayerHook("SimplePlayer::updatePlayerFrame", enabled);
                break;
            case 3: // sog buttons
                sogutils::toggleLayerHook("CCMenuItemSpriteExtra::create", enabled);
                break;
            case 4: // sog bg
                sogutils::toggleLayerHook("cocos2d::CCSprite::create", enabled);
                break;
        }
    }
    std::vector<ShopCategory> getCategories();
    std::vector<ShopItem> getItems();
    std::vector<Quest> getQuests();
}
