#include "SoggifyMenuLayer.hpp"
#include "Geode/ui/Popup.hpp"
#include "QuestsPopup.hpp"
#include "ShopLayer.hpp"
#include <games/MGPopup.hpp>
#include <defs/geode.hpp>
#include <defs/utils.hpp>

using namespace geode::prelude;

SoggifyMenuLayer* SoggifyMenuLayer::create() {
    auto ret = new SoggifyMenuLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

CCScene* SoggifyMenuLayer::scene() {
    auto layer = SoggifyMenuLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

void SoggifyMenuLayer::showPopup(float) {
    FLAlertLayer::create(nullptr, "Using Soggify", "Welcome to the <cy>Soggify Mod!</c> The mod that <cy>sogs</c> the entirety of <cg>GD!</c> Right now, you haven't unlocked it's full potential!\nTo unlock more features of this mod, you will have to <cb>complete quests</c> or <cp>play mini games</c> to gain <cg>Sog Points</c>! Those points can be used in a <cp>shop</c> to buy those features to <cy>sog</c> your game even more!\nYou can always show this popup on the <co>top right</c>. Good luck <cy>soggifying</c> your game!", "OK", nullptr, 400.f, false, 150.f, 1.0f)->show();
}

std::string getNodeName(cocos2d::CCObject* node) {
#ifdef GEODE_IS_WINDOWS
        return typeid(*node).name() + 6;
#else 
        {
            std::string ret;

            int status = 0;
            auto demangle = abi::__cxa_demangle(typeid(*node).name(), 0, 0, &status);
            if (status == 0) {
                ret = demangle;
            }
            free(demangle);

            return ret;
        }
#endif
}

// hook jumpscare
class FunnyJumpscare : public CCNode {
    public:
        void bye2() {
            log::info("ok lol bye");
            this->removeFromParentAndCleanup(true);
            if (auto ML = MenuLayer::get()) {
                ML->onCreator(nullptr);
            }
        }
        bool init() {
            if (!CCNode::init()) return false;
            runAction(CCSequence::create(
                CCDelayTime::create(0.5F),
                CCCallFunc::create(this, callfunc_selector(FunnyJumpscare::bye2)),
                nullptr
            ));
            return true;
        }
        static FunnyJumpscare* create() {
            auto pRet = new FunnyJumpscare();
            if (pRet && pRet->init()) {
                pRet->autorelease();
                return pRet;
            }
            CC_SAFE_DELETE(pRet);
            return nullptr;
        }
};

void SoggifyMenuLayer::jumpscare(float) {
    log::info("jumpscare");
    CCDirector::sharedDirector()->setFastMenu(true);
    onClose();
    sogutils::toggleLayerHook("CreatorLayer::init", true);
    sogutils::toggleLayerHook("LevelSearchLayer::init", true);
    sogutils::toggleLayerHook("LevelBrowserLayer::loadLevelsFinished", true);
    sogutils::toggleLayerHook("LevelInfoLayer::updateLabelValues", true);
    sogutils::toggleLayerHook("LevelInfoLayer::levelDownloadFinished", true);
    if (auto ML = MenuLayer::get()) {
        ML->addChild(FunnyJumpscare::create());
    }
}
// hook jumpscare

// where the black magic happens
bool SoggifyMenuLayer::init() {
    if (!CCLayer::init()) return false;
    sogutils::toggleLayerHook("DialogLayer::displayDialogObject", true);
    if (!Mod::get()->hasSavedValue("new1")) {
        scheduleOnce(schedule_selector(SoggifyMenuLayer::showPopup), 0.5f);
        Mod::get()->setSavedValue("new1", true);
    }
    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();
    // this is so cursed
    Build<CCSprite>::create(GameManager::get()->getBGTexture(26))
        .pos(0, -50)
        .color({0, 100, 200})
        .zOrder(-1)
        .anchorPoint(0, 0)
        .parent(this)
        .runAction(Build<CCSequence>::create(
            CCEaseInOut::create(CCMoveBy::create(3.0f, ccp(0, 5)), 2.0f),
            CCEaseInOut::create(CCMoveBy::create(3.0f, ccp(0, -5)), 2.0f)
        ).repeatForever())
        .scaleToMatchX(size.width);
 
    Build<CCMenu>::create().pos(0,0).parent(this).store(m_buttonMenu);
    Build<CCMenu>::create().parent(this).store(m_mainBtns);

    Build<CCSprite>::createSpriteName("GJ_arrow_03_001.png")
        .intoMenuItem([this]() {
            this->onClose();
        })
        .parent(m_buttonMenu)
        .pos({25, size.height - 25});
    Build<CCSprite>::createSpriteName("GJ_infoIcon_001.png")
        .scale(1.1f)
        .intoMenuItem([this]() {
            showPopup(0.f);
        })
        .parent(m_buttonMenu)
        .pos({size.width - 25, size.height - 25});

    // === Buttons === //
    Build<CCSprite>::create("SOGGY.png"_spr)
        .scale(1.2f)
        .runAction(
            Build<CCSequence>::create(
                CCEaseInOut::create(CCMoveBy::create(0.5f, ccp(0, 2)), 2.0f),
                CCEaseInOut::create(CCMoveBy::create(0.5f, ccp(0, -2)), 2.0f)
            ).repeatForever()
        )
        .intoMenuItem([this]() {
            std::vector<std::string> dialogues = {
                "meow<d010> why u keep clicking,<d030> go do <cp>quests</c> or play <cg>mini games</c>!<d050> or buy something...",
                "go away",
                "mow",
                "meow",
                "<s300>meow</s>",
                "<d100>mow",
                "<i100>mow</i>"
            };
            int rInd = rand() % dialogues.size();
            CCArray* dialogArr = CCArray::create();
            int rand2 = rand() % 5;
            switch (rand2) {
                default:
                    FMODAudioEngine::sharedEngine()->playEffect("s2961.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume);
                    break;
                case 2:
                    FMODAudioEngine::sharedEngine()->playEffect("s2962.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume);
                    break;
                case 3:
                    FMODAudioEngine::sharedEngine()->playEffectAdvanced("s2961.ogg"_spr, 1.1f, 0, GameManager::get()->m_sfxVolume, 1.005f, true, false, 0, 0, 0, 0, false, 0, false, true, 0, 0, 0, 0);
                    break;
                case 4:
                    FMODAudioEngine::sharedEngine()->playEffectAdvanced("s2962.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume, 1.005f, true, false, 0, 0, 0, 0, false, 0, false, true, 0, 0, 0, 0);
                    break;
            }
            DialogObject* soggyDialog;
            if (!Mod::get()->hasSavedValue("new02")) {
                soggyDialog = DialogObject::create("soggy", "mow<d060> here is a hint to get more <cy>points</c>,<d030> you have <cy>Soggy Mod</c>,<d060> <cg>go to it</c>!!!", 2, 1.0f, false, {255, 255, 255});
                Mod::get()->setSavedValue("new02", true);
            } else {
                soggyDialog = DialogObject::create("soggy", dialogues[rInd], 2, 1.0f, false, {255, 255, 255});
            }
            soggyDialog->setTag(100125);
            dialogArr->addObject(soggyDialog);
            DialogLayer* dialogLayer = DialogLayer::createWithObjects(dialogArr, 2);
            this->addChild(dialogLayer);
            dialogLayer->animateInRandomSide();
        })
        .parentAtPos(m_mainBtns, Anchor::Center, {0, 30})
        .collect();
    Build<ButtonSprite>::create("Quests", "bigFont.fnt", "GJ_button_01.png")
        .scale(0.8f)
        .intoMenuItem([]() {
            QuestsPopup::create()->show();
        })
        .parentAtPos(m_mainBtns, Anchor::Bottom, {0, 90});
    Build<ButtonSprite>::create("Mini Games", "bigFont.fnt", "GJ_button_02.png")
        .scale(0.8f)
        .intoMenuItem([]() {
            MGPopup::create()->show();
        })
        .visible(Mod::get()->hasSavedValue("i_8") && Mod::get()->getSavedValue<bool>("i_8"))
        .store(m_minigamesBtn)
        .parentAtPos(m_mainBtns, Anchor::Bottom, {0, 20}); // 55
    Build<ButtonSprite>::create("Shop", "bigFont.fnt", "GJ_button_04.png")
        .scale(0.8f)
        .intoMenuItem([this]() {
            CCDirector::get()->pushScene(CCTransitionFade::create(.5f, ShopLayer::scene()));
        })
        .parentAtPos(m_mainBtns, Anchor::Bottom, {0, 55}); // 20
    Build<CCSprite>::create("funnybutton.png"_spr)
        .intoMenuItem([this]() {
            geode::createQuickPopup("Permission Request", "<cy>Sog</c> wants permission to <cr>take control</c> of your inputs for <cg>Geometry Dash</c>.\nWould you like to give <cy>Sog</c> permission?", "NO!", "sure why not", [this](FLAlertLayer *, bool btn2) {
                if (btn2) {
                    CCArray* dialogArr = CCArray::create();
                    FMODAudioEngine::sharedEngine()->playEffectAdvanced("s2961.ogg"_spr, 1.0f, 0, GameManager::get()->m_sfxVolume, 0.990f, true, false, 0, 0, 0, 0, false, 0, false, true, 0, 0, 0, 0);
                    DialogObject* soggyDialog = DialogObject::create("soggy", "meow<d070> thank you for giving <cy>sog</c> permission<d050>.<d050>.<d050>.<cr>GOODBYE</c>", 2, 1.0f, false, {255, 255, 255});
                    soggyDialog->setTag(100125);
                    dialogArr->addObject(soggyDialog);
                    DialogLayer* dialogLayer = DialogLayer::createWithObjects(dialogArr, 2);
                    this->addChild(dialogLayer);
                    dialogLayer->animateInRandomSide();
                    Notification::create("Permission Granted.", NotificationIcon::Success)->show();
                    this->scheduleOnce(schedule_selector(SoggifyMenuLayer::jumpscare), 3.5f);
                } else {
                    Notification::create("Sog tried to tap Escape but has no permission!", NotificationIcon::Error)->show();
                }
            }, true, false);
        })
        .parentAtPos(m_mainBtns, Anchor::BottomLeft, {25, 25})
        .visible(Mod::get()->hasSavedValue("i_7") && Mod::get()->getSavedValue<bool>("i_7"))
        .store(m_funnyBtn);
    Build<CCLabelBMFont>::create("", "goldFont.fnt")
        .parentAtPos(m_mainBtns, Anchor::Top, {0, -25})
        .store(m_pointLabel);

    updatePoints();
    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    if ((rand() % 40) < 20) {
        GameManager::sharedState()->fadeInMusic("capeling.soggy-mod/SogLoop.mp3");
    } else {
        GameManager::sharedState()->fadeInMusic("capeling.soggy-mod/SogLoop02.mp3");
    }
    this->setID("SoggifyMenuLayer"_spr);
    return true;
}

void SoggifyMenuLayer::updatePoints() {
    m_pointLabel->setString(fmt::format("{} sog points", Mod::get()->getSavedValue<int>("points", 0)).c_str());
}

// to update points upon return idk if this is necessary
void SoggifyMenuLayer::onEnter() {
    CCLayer::onEnter();
    m_funnyBtn->setVisible(Mod::get()->hasSavedValue("i_7") && Mod::get()->getSavedValue<bool>("i_7"));
    m_minigamesBtn->setVisible(Mod::get()->hasSavedValue("i_8") && Mod::get()->getSavedValue<bool>("i_8"));
    //m_funnyBtn->setVisible(Mod::get()->hasSavedValue("i_11") && Mod::get()->getSavedValue<bool>("i_11"));
    updatePoints();
}
void SoggifyMenuLayer::onClose() {
    sogutils::toggleLayerHook("DialogLayer::displayDialogObject", false);
    sogutils::toggleLayerHook("DialogLayer::onClose", false);
    //soggy::utils::toggleLayerHook(false);
    GameManager::sharedState()->fadeInMenuMusic();
    //CCDirector::get()->popSceneWithTransition(0.5F, PopTransition::kPopTransitionFade);
    CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f,MenuLayer::scene(false)));
}

// achievement ideas
// find sog (hide and seek)
// complete a quest
// beat sog at pong
// gain sog points from soggy (the soggy mod easter egg)
