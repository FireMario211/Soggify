#include "ShopItemLayer.hpp"
#include "Geode/ui/Popup.hpp"
#include "NukeLayer.hpp"
#include <defs/geode.hpp>
#include <defs/utils.hpp>

using namespace geode::prelude;

ShopItemLayer* ShopItemLayer::create(ShopItem item) {
    auto ret = new ShopItemLayer();
    if (ret->init(item)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

CCScene* ShopItemLayer::scene(ShopItem item) {
    auto layer = ShopItemLayer::create(item);
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

bool ShopItemLayer::init(ShopItem item) {
    if (!CCLayer::init()) return false;
    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();
    // this is so cursed
    Build<CCLayerColor>::create()
        .color({255,255,255})
        .opacity(255)
        .zOrder(-1)
        .anchorPoint(0, 0)
        .parent(this)
        .scaleToMatchX(size.width)
        .with([this](CCLayerColor* layer) {
            auto height = layer->getContentHeight();
            // glowy corners
            Build<CCSprite>::createSpriteName("d_gradient_01_001.png")
                .anchorPoint(0.5, 0)
                .rotation(90)
                .color(188, 231, 253)
                .scaleY(2.5f)
                .scaleX(20.f)
                .parentAtPos(layer, Anchor::Left);
            Build<CCSprite>::createSpriteName("d_gradient_01_001.png")
                .anchorPoint(0.5, 0)
                .rotation(-90)
                .color(188, 231, 253)
                .scaleY(2.5f)
                .scaleX(20.f)
                .parentAtPos(layer, Anchor::Right);
            Build<CCLabelBMFont>::create("Details", "PGothic.fnt"_spr)
                .scale(0.8f)
                .anchorPoint(0.5, 1)
                .color(40,40,40)
                .parentAtPos(layer, Anchor::Top, {-140, -20});
            
            // 565
            // idk if i use CCSpriteBatchNode, or if theres a better way of doing this
            for (int i = 0; i < 120; i++) {
                Build<CCSprite>::create("circle.png")
                    .scale(0.15f)
                    .color(100,100,100)
                    .parentAtPos(layer, Anchor::TopLeft, {(float)(i * 5) - 5, -50});
                Build<CCSprite>::create("circle.png")
                    .scale(0.15f)
                    .color(100,100,100)
                    .parentAtPos(layer, Anchor::BottomLeft, {(float)(i * 5) - 5, 70});
            }
        });
    Build<CCMenu>::create().anchorPoint(0.5,0.5).parent(this).zOrder(1).store(m_buttonMenu);

    auto bg = sogutils::createShopButton("", {360, 180}, false);
    auto imgbg = sogutils::createShopButton("", {140, 100}, false);
    Build<CCSprite>::create(fmt::format("preview{}.png"_spr, item.id).c_str())
        .scaleToMatchX(imgbg->getContentWidth() - 3)
        .scaleToMatchY(imgbg->getContentHeight() - 3)
        .parentAtPos(imgbg, Anchor::Center);
    imgbg->setAnchorPoint({0, 0.5});
    auto desc = TextArea::create(
        item.desc, "PGothic.fnt"_spr,
        0.4f,
        170.F, { 0.0f, 0.5f },
        15.f,
        false
    );
    desc->colorAllCharactersTo({40, 40, 40});
    desc->setAnchorPoint({0, 1});
    bg->addChildAtPosition(desc, Anchor::Top, {-20, -10});
    bg->addChildAtPosition(imgbg, Anchor::Left, {10, 25});
    auto line = BreakLine::create(340, 2.f, {105.f / 255.f, 205.f / 255.f, 240.f / 255.f, 1.f});
    line->setAnchorPoint({0.5, 0.5});
    bg->addChildAtPosition(line, Anchor::Bottom, {0, 55});
    Build<CCLabelBMFont>::create(item.name.c_str(), "PGothic.fnt"_spr)
        .scale(0.5f)
        .color(105,205,240)
        .parentAtPos(bg, Anchor::Bottom, {0, 40});
    Build<CCLabelBMFont>::create(fmt::format("{} Sog Points", item.price).c_str(), "PGothic.fnt"_spr)
        .scale(0.4f)
        .color(40,40,40)
        .parentAtPos(bg, Anchor::Bottom, {0, 10});
    m_buttonMenu->addChildAtPosition(bg, Anchor::Center, {0, 10});

    bool boughtItem = Mod::get()->hasSavedValue(fmt::format("i_{}", item.id));
    if (boughtItem) {
        bool enabled = Mod::get()->getSavedValue<bool>(fmt::format("i_{}", item.id));
        const char* onBtn = (enabled) ? "GJ_checkOn_001.png" : "GJ_checkOff_001.png";
        const char* offBtn = (enabled) ? "GJ_checkOff_001.png" : "GJ_checkOn_001.png";
        m_buttonMenu->addChildAtPosition(CCMenuItemExt::createTogglerWithFrameName(offBtn, onBtn, 1.0, [item](auto) {
            Mod::get()->setSavedValue(fmt::format("i_{}", item.id), !Mod::get()->getSavedValue<bool>(fmt::format("i_{}", item.id)));
            bool enabled = Mod::get()->getSavedValue<bool>(fmt::format("i_{}", item.id));
            sogutils::updateHooks(item.id, enabled);
        }), Anchor::Bottom, {120, 45});
    } else {
        m_buttonMenu->addChildAtPosition(CCMenuItemExt::createSpriteExtra(sogutils::createShopButton("Purchase"), [this, item](auto) {
            geode::createQuickPopup("Buy Item", fmt::format("Do you want to <cg>buy</c> <co>{}</c> for\n<cy>{} Sog Points</c>?\n<cr>NO REFUNDS</c>", item.name, item.price), "Cancel", "Buy", [this, item](auto, bool buyIt) {
                if (!buyIt) return;
                // just in case, idk if this item is really stable after seeing the results
                if (item.id == 6) {
                    geode::createQuickPopup("nope", "do you really want to <cr>break your game</c>", "nuh uh", "yes i do", [this, item](auto, auto btn2) {
                        if (btn2) {
                            FMODAudioEngine::sharedEngine()->playEffect("INCORRECT.mp3"_spr, 1, 0, GameManager::get()->m_sfxVolume);
                            FLAlertLayer::create("hA", "well <cr>TOO BAD</c>! <cb>someone</c> will ask how to disable sog everything, see the problem? ok bye", "mow")->show();
                        }
                    }, true, false);
                    return;
                }
                // nuke
                if (item.id == 9) {
                    if (Mod::get()->getSavedValue<int>("points") < item.price) return FLAlertLayer::create("Error", "You do not have enough <cy>sog points</c> to buy this item!", "OK")->show();
                    geode::createQuickPopup("A warning", "Are you <cr>REALLY</c> sure you want to <cg>buy</c> <co>nuke</c>?", "nope", "yes", [this, item](auto, bool buyIt2) {
                        if (buyIt2) {
                            geode::createQuickPopup("Last chance!", "Are you <cr>REALLY REALLY REALLY</c> sure you want to <cg>buy</c> <co>nuke</c>?\n<cy>Sog</c> WILL be mad at you, and <cr>you will lose all progress on this mod if you buy this.</c>", "ok ur right i wont", "im sure", [this, item](auto, bool buyIt3) {
                                if (buyIt3) {
                                    CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f,NukeLayer::scene()));
                                }
                            });
                        }
                    });
                    return;
                }

                if (item.id == 10) {
                    if (Mod::get()->getSavedValue<int>("points") < item.price) return FLAlertLayer::create("Error", "You do not have enough <cy>sog points</c> to buy this item!", "OK")->show();
                    FLAlertLayer::create("Purchased!", "You bought <cy>nothing</c>, a waste of points!", "OK")->show();
                    Mod::get()->setSavedValue("points", Mod::get()->getSavedValue<int>("points") - item.price);
                    FMODAudioEngine::sharedEngine()->playEffect("buyItem03.ogg", 1, 0, GameManager::get()->m_sfxVolume);
                    m_pointLabel->setString(fmt::to_string(Mod::get()->getSavedValue<int>("points")).c_str());
                    return;
                }
                if (Mod::get()->hasSavedValue(fmt::format("i_{}", item.id))) {
                    FLAlertLayer::create("Error", "You have already <cy>bought this item!</c>\nGo back and view this item again to toggle it!", "OK")->show();
                } else {
                    if (Mod::get()->getSavedValue<int>("points") < item.price) return FLAlertLayer::create("Error", "You do not have enough <cy>sog points</c> to buy this item!", "OK")->show();
                    Mod::get()->setSavedValue("points", Mod::get()->getSavedValue<int>("points") - item.price);
                    FMODAudioEngine::sharedEngine()->playEffect("buyItem03.ogg", 1, 0, GameManager::get()->m_sfxVolume);
                    m_pointLabel->setString(fmt::to_string(Mod::get()->getSavedValue<int>("points")).c_str());
                    geode::createQuickPopup("Purchased!", "Would you like to <cg>enable</c> or <cr>disable</c> this item?", "Disable", "Enable", [this, item](auto, auto btn2) {
                        sogutils::updateHooks(item.id, btn2);
                        Mod::get()->setSavedValue(fmt::format("i_{}", item.id), btn2);
                        onClose();
                    }, true, false);
                }
            }, true, false);
        }), Anchor::Bottom, {120, 45});
    }

    m_buttonMenu->addChildAtPosition(CCMenuItemExt::createSpriteExtra(sogutils::createShopButton("Back"), [this](auto) {
        onClose();
    }), Anchor::Bottom, {-120, 45});
    Build<CCLabelBMFont>::create(fmt::to_string(Mod::get()->getSavedValue<int>("points")).c_str(), "PGothic.fnt"_spr)
        .scale(0.5f)
        .store(m_pointLabel)
        .alignment(CCTextAlignment::kCCTextAlignmentCenter)
        .color(52, 190, 237)
        .parentAtPos(m_buttonMenu, Anchor::Bottom, {0, 50});
    Build<CCLabelBMFont>::create("Sog Points", "PGothic.fnt"_spr)
        .scale(0.4f)
        .alignment(CCTextAlignment::kCCTextAlignmentCenter)
        .color(40, 40, 40)
        .parentAtPos(m_buttonMenu, Anchor::Bottom, {0, 35});
    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
    return true;
}

void ShopItemLayer::onEnter() {
    CCLayer::onEnter();
    m_pointLabel->setString(fmt::to_string(Mod::get()->getSavedValue<int>("points")).c_str());
}

void ShopItemLayer::onClose() {
    CCDirector::get()->popScene();
}
