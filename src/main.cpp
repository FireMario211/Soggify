// NOT AS EMPTY...
#include <defs/utils.hpp>
#include <Geode/modify/DialogLayer.hpp>
#include <capeling.soggy-mod/include/Events.hpp>
using namespace geode::prelude;

$on_mod(Loaded) {
    sogutils::toggleLayerHook("DialogLayer::displayDialogObject", false);
    sogutils::toggleLayerHook("DialogLayer::onClose", false);
};

int step = -1;

// TODO for fixing jitless:
// fix DialogLayer::displayDialogObject (weird nullptr crash)
// fix CCSprite::create
// fix CCMenuItemSpriteExtra::create?
// test Jumpscare
// test GSM

// ty cologne
class $modify(SoggifyHookDL, DialogLayer) {
    void displayDialogObject(DialogObject* dialogObject) {
        DialogLayer::displayDialogObject(dialogObject);
        if (dialogObject->getTag() == 100125) {
            m_characterSprite->initWithFile("soggyDialogue_001.png"_spr);
            return;
        } else if (dialogObject->getTag() == 100425) {
            m_characterSprite->initWithFile("soggyDialogue_001.png"_spr);
        } else if (dialogObject->getTag() == 1004251) {
            FMODAudioEngine::sharedEngine()->playEffect("s2961.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume);
            m_characterSprite->initWithFile("soggyDialogue_001.png"_spr);
        } else if (dialogObject->getTag() == 100426) {
            m_characterSprite->initWithFile("soggyDialogue_002.png"_spr);
            m_textArea->colorAllLabels({255, 50, 50});
        } else if (dialogObject->getTag() == 100225) {
            m_characterSprite->initWithFile("soggyDialogue_002.png"_spr);
            m_textArea->colorAllLabels({255, 50, 50});
            return;
        } else if (dialogObject->getTag() == 1004267) {
            FMODAudioEngine::sharedEngine()->playEffectAdvanced("s2962.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume, 0.75f, true, false, 0, 0, 0, 0, false, 0, false, true, 0, 0, 0, 0);
            m_characterSprite->initWithFile("soggyDialogue_002.png"_spr);
            m_textArea->colorAllLabels({255, 50, 50});
        } else if (dialogObject->getTag() == 1004268) {
            FMODAudioEngine::sharedEngine()->playEffectAdvanced("s2962.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume, 0.75f, true, false, 0, 0, 0, 0, false, 0, false, true, 0, 0, 0, 0);
            m_characterSprite->initWithFile("soggyDialogue_002.png"_spr);
            m_textArea->colorAllLabels({255, 50, 50});
            return;
        }
        sogutils::toggleLayerHook("DialogLayer::displayDialogObject", false);
    }
    void onClose() {
        DialogLayer::onClose();
        switch (step) {
            case 0:
                if (!Mod::get()->getSavedValue<bool>("e01")) {

                }
                break;
            case 4:
                if (!Mod::get()->getSavedValue<bool>("e02")) {

                }
                break;
        }
        if (Mod::get()->getSavedValue<bool>("nuke01")) {
            sogutils::toggleLayerHook("DialogLayer::displayDialogObject", true);
            sogutils::toggleLayerHook("DialogLayer::onClose", true);
            Mod::get()->setSavedValue<bool>("nuke01", false);
            Mod::get()->setSavedValue<bool>("nuke02", true);
            Mod::get()->setSavedValue("points", 0);
            if (Mod::get()->hasSavedValue("i_2")) {
                Mod::get()->getSaveContainer().erase("i_2");
                sogutils::updateHooks(2, false);
            }
            if (Mod::get()->hasSavedValue("i_3")) {
                Mod::get()->getSaveContainer().erase("i_3");
                sogutils::updateHooks(3, false);
            }
            if (Mod::get()->hasSavedValue("i_4")) {
                Mod::get()->getSaveContainer().erase("i_4");
                sogutils::updateHooks(4, false);
            }
            for (int i = 4; i < 10; i++) {
                auto key = fmt::format("i_{}", i);
                if (Mod::get()->hasSavedValue(key)) {
                    Mod::get()->getSaveContainer().erase(key);
                }
            }
            GameManager::sharedState()->fadeInMenuMusic();
            CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f,MenuLayer::scene(false)));
            return;
        }
        sogutils::toggleLayerHook("DialogLayer::onClose", false);
    }
};


$execute {
    new EventListener<EventFilter<soggy_mod::OnSogLayer>>(+[](soggy_mod::OnSogLayer* e) {
        if (!(!Mod::get()->hasSavedValue("e01") && !Mod::get()->hasSavedValue("e02") && !Mod::get()->hasSavedValue("e03"))) {
            return ListenerResult::Propagate;
        }
        if (Mod::get()->getSettingValue<bool>("ery-mode")) return ListenerResult::Propagate;
        if (step == -1) {
            step = 0;
        }
        auto size = CCDirector::sharedDirector()->getWinSize();
        auto layer = e->getLayer();
        int rand = e->getRandom();
        log::debug("WE HOOKED THE SOGG!!! with rand of {}", rand);
        if (step == 2 && rand > 90) {
            step++;
        }
        CCMenu* buttonMenu = CCMenu::create();
        auto btn = CCMenuItemExt::createSpriteExtra(CircleButtonSprite::createWithSpriteFrameName("gjItem_04_001.png"), [layer, rand](CCObject*) {
            sogutils::toggleLayerHook("DialogLayer::displayDialogObject", true);
            sogutils::toggleLayerHook("DialogLayer::onClose", true);
            CCArray* dialogArr = CCArray::create();
            DialogObject* soggyDialog;
            if (rand == 46) { // evil
                soggyDialog = DialogObject::create("evil soggy", "<s300>YOU MADE THE SOG ANGRY</s>", 2, 1.2f, false, {255, 50, 50});
                soggyDialog->setTag(100426);
                FMODAudioEngine::sharedEngine()->playEffectAdvanced("s2962.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume, 0.5f, true, false, 0, 0, 0, 0, false, 0, false, true, 0, 0, 0, 0);
            } else if (Mod::get()->getSavedValue<bool>("e01")) {
                switch (step) {
                    case 0: {
                        FMODAudioEngine::sharedEngine()->playEffectAdvanced("s2962.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume, 0.75f, true, false, 0, 0, 0, 0, false, 0, false, true, 0, 0, 0, 0);
                        auto d2 = DialogObject::create("soggy", "<s200>YOUVE ALREADY GOTTEN YOUR REWARD.</s>", 2, 1.1f, false, {255, 255, 255});
                        d2->setTag(100225);
                        soggyDialog = DialogObject::create("soggy", "now out!<d020> there are no more easter eggs", 2, 1.f, false, {255, 255, 255});
                        soggyDialog->setTag(1004251);
                        dialogArr->addObject(d2);
                        break;
                    }
                    case 1:
                        FMODAudioEngine::sharedEngine()->playEffect("s2962.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume);
                        soggyDialog = DialogObject::create("soggy", "what u want <d50> u want <cy>more sog points?</c>", 2, 0.5f, false, {255, 255, 255});
                        soggyDialog->setTag(100425);
                        break;
                    case 2:
                        FMODAudioEngine::sharedEngine()->playEffect("s2961.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume);
                        soggyDialog = DialogObject::create("soggy", "ok fine! wait for <cy>sog</c> to <cp>move</c>", 2, 1.0f, false, {255, 255, 255});
                        soggyDialog->setTag(100425);
                        break;
                    case 3:
                        if (Mod::get()->getSavedValue<bool>("e03")) { // CHEATER
                            FMODAudioEngine::sharedEngine()->playEffect("s2962.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume);
                            auto d2 = DialogObject::create("soggy", "u really spent time doing that<d010>.<d010>.<d010>. ok <cy>sog</c> will reward you with<d010>.<d010>.<d010>. WAIT.", 2, 1.0f, false, {255, 255, 255});
                            d2->setTag(100125);
                            soggyDialog = DialogObject::create("soggy", "<s200>YOU CHEATER YOU'RE GETTING NOTHING!</s>", 2, 1.f, false, {255, 255, 255});
                            soggyDialog->setTag(1004267);
                            dialogArr->addObject(d2);
                            Mod::get()->setSavedValue("e02", true);
                            Mod::get()->setSavedValue("e01", false);
                        } else {
                            FMODAudioEngine::sharedEngine()->playEffect("s2962.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume);
                            //soggyDialog = DialogObject::create("soggy", "u really spent time doing that<d010>.<d010>.<d010>. ok <cy>sog</c> will reward you with <cy>1000 sog points</c>, and a <cr>harder mode</c> if you battle <cy>sog</c>, mow", 2, 0.8f, false, {255, 255, 255});
                            soggyDialog = DialogObject::create("soggy", "u really spent time doing that<d010>.<d010>.<d010>. ok <cy>sog</c> will reward you with <cy>1000 sog points</c>, and also tell you to wait for a future update! mow", 2, 0.8f, false, {255, 255, 255});
                            soggyDialog->setTag(100425);
                            Mod::get()->setSavedValue("points", Mod::get()->getSavedValue<int>("points", 0) + 1000);
                            Mod::get()->setSavedValue("e03", true);
                        }
                        break;
                    case 4:
                        FMODAudioEngine::sharedEngine()->playEffect("s2961.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume);
                        soggyDialog = DialogObject::create("soggy", "there happy?<d050> no more!<d070> now tap <cy>sog</c> nose", 2, 1.0f, false, {255, 255, 255});
                        soggyDialog->setTag(100425);
                        break;
                    default:
                        soggyDialog = DialogObject::create("soggy", "<d010>.<d010>.<d010>.", 2, 1.0f, false, {255, 255, 255});
                        soggyDialog->setTag(100425);
                        Mod::get()->setSavedValue("e02", true);
                        Mod::get()->setSavedValue("e01", false);
                        break;
                }
                if (step == 2) {
                    if (rand > 90) {
                        step++;
                    }
                } else {
                    step++;
                }
            } else if (Mod::get()->getSavedValue<bool>("e02")) {
                soggyDialog = DialogObject::create("soggy", "<d010>.<d010>.<d010>.", 2, 1.0f, false, {255, 255, 255});
                soggyDialog->setTag(100425);
            } else {
                if (!Mod::get()->hasSavedValue("e01") && !Mod::get()->hasSavedValue("e02") && !Mod::get()->hasSavedValue("e03")) {
                    //1000
                    soggyDialog = DialogObject::create("soggy", "<i070>mow,</i><d070> there is your <cy>500 sog points</c> for finding sog", 2, 1.f, false, {255, 255, 255});
                    soggyDialog->setTag(100125);
                    FMODAudioEngine::sharedEngine()->playEffect("s2961.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume);
                    Mod::get()->setSavedValue("points", Mod::get()->getSavedValue<int>("points", 0) + 500);
                    Mod::get()->setSavedValue("e01", true);
                } else {
                    FMODAudioEngine::sharedEngine()->playEffect("s2962.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume);
                    soggyDialog = DialogObject::create("soggy", "cheater", 2, 0.5f, false, {255, 255, 255});
                    soggyDialog->setTag(100425);
                }
            }
            dialogArr->addObject(soggyDialog);
            DialogLayer* dialogLayer = DialogLayer::createWithObjects(dialogArr, 2);
            layer->getParent()->addChild(dialogLayer);
            dialogLayer->animateInRandomSide();
        });
        btn->setPosition({0, (-size.height / 2) + 20});
        buttonMenu->addChild(btn);
        layer->addChild(buttonMenu);
        return ListenerResult::Propagate;
    });
}
