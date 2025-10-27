#include <defs/geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <layers/SoggifyMenuLayer.hpp>
#include <defs/utils.hpp>

CCParticleSystemQuad* s_emitter = nullptr;

bool changedSetting = false;

class $modify(SoggifyHookML, MenuLayer) {
    static void onModify(auto& self) {
        (void)self.setHookPriority("MenuLayer::init", Priority::First);
    }
    void createParticle(bool enable) {
        if (s_emitter) {
            if (changedSetting) {
                log::debug("forgetting emitter");
                changedSetting = false;
                s_emitter->removeFromParentAndCleanup(true);
                SceneManager::get()->forget(s_emitter);
                s_emitter = nullptr;
                return createParticle(enable);
            }
            s_emitter->setVisible(enable);
            return;
        }
        int scale = Mod::get()->getSettingValue<int>("sog-rain-scale");
        auto dict = CCDictionary::createWithContentsOfFileThreadSafe("dragEffect.plist");

        dict->setObject(CCString::create("0"), "emitterType");
        dict->setObject(CCString::create("-1"), "duration");
        dict->setObject(CCString::createWithFormat("%i", Mod::get()->getSettingValue<int>("sog-rain-count")), "maxParticles");
        dict->setObject(CCString::create("1.5"), "particleLifespan");
        dict->setObject(CCString::create("0.3"), "particleLifespanVariance");
        dict->setObject(CCString::create("90"), "angle");
        dict->setObject(CCString::create("90"), "angleVariance");
        dict->setObject(CCString::create("100"), "sourcePositionx");
        dict->setObject(CCString::create("0"), "sourcePositiony");
        dict->setObject(CCString::create("300"), "sourcePositionVariancex");
        dict->setObject(CCString::create("0"), "sourcePositionVariancey");
        dict->setObject(CCString::create("0"), "maxRadius");
        dict->setObject(CCString::create("0"), "minRadius");
        dict->setObject(CCString::create(std::to_string(scale)), "startParticleSize");
        dict->setObject(CCString::create(std::to_string(scale - 10)), "finishParticleSize");
        dict->setObject(CCString::create("3"), "startParticleSizeVariance");
        dict->setObject(CCString::create("0"), "speed");
        dict->setObject(CCString::create("0"), "speedVariance");
        dict->setObject(CCString::create("1"), "gravityx");
        dict->setObject(CCString::create(std::to_string(Mod::get()->getSettingValue<int>("sog-rain-gravity"))), "gravityy");
        dict->setObject(CCString::create("1"), "startColorRed");
        dict->setObject(CCString::create("1"), "startColorGreen");
        dict->setObject(CCString::create("1"), "startColorBlue");
        dict->setObject(CCString::create("1"), "startColorAlpha");
        dict->setObject(CCString::create("0"), "startColorVarianceRed");
        dict->setObject(CCString::create("0"), "startColorVarianceBlue");
        dict->setObject(CCString::create("0"), "startColorVarianceGreen");
        dict->setObject(CCString::create("0"), "startColorVarianceAlpha");
        dict->setObject(CCString::create("1"), "finishColorRed");
        dict->setObject(CCString::create("1"), "finishColorGreen");
        dict->setObject(CCString::create("1"), "finishColorBlue");
        dict->setObject(CCString::create("1"), "finishColorAlpha");
        dict->setObject(CCString::create("0"), "finishColorVarianceRed");
        dict->setObject(CCString::create("0"), "finishColorVarianceBlue");
        dict->setObject(CCString::create("0"), "finishColorVarianceGreen");
        dict->setObject(CCString::create("0"), "finishColorVarianceAlpha");
        dict->setObject(CCString::create("0"), "rotatePerSecond");
        dict->setObject(CCString::create("0"), "rotatePerSecondVariance");
        dict->setObject(CCString::create("0"), "rotationEndVariance");
        dict->setObject(CCString::create("square.png"), "textureFileName");

        s_emitter = CCParticleSystemQuad::create();
        s_emitter->initWithDictionary(dict, false);
        s_emitter->setTexture(CCSprite::create("SOGGY.png"_spr)->getTexture());
        auto winSize = CCDirector::get()->getWinSize();
        s_emitter->setZOrder(CCScene::get()->getHighestChildZ() + 1);
        s_emitter->setPosition({winSize.width / 2, winSize.height + 20});
        s_emitter->setVisible(enable);
        SceneManager::get()->keepAcrossScenes(s_emitter);
    }
    bool init() {
        bool enableSogButtons = Mod::get()->hasSavedValue("i_3") && Mod::get()->getSavedValue<bool>("i_3");
        bool enableSogEverything = Mod::get()->hasSavedValue("i_6") && Mod::get()->getSavedValue<bool>("i_6");
        //sogutils::toggleLayerHook("cocos2d::CCScale9Sprite::setContentSize", enableSogEverything);
        //sogutils::toggleLayerHook("cocos2d::CCSprite::create", enableSogEverything);
        //sogutils::toggleLayerHook("cocos2d::CCSprite::createWithSpriteFrameName", enableSogEverything);
        if (!MenuLayer::init()) return false;
        if (Mod::get()->getSettingValue<bool>("ery-mode")) return true;
        if (Mod::get()->hasSavedValue("i_5")) {
            createParticle(Mod::get()->getSavedValue<bool>("i_5"));
        }
        if (auto menu = this->getChildByID("right-side-menu")) {
            Build<CCSprite>::create("SoggyBtn.png"_spr)
                .intoMenuItem([this]() {
                    //"Wait a minute, I don't remember this being here..."
                    //"MOW"
                    //"mow." (potbor's head gets replaced with sog)
                    this->willClose();
                    CCDirector::get()->replaceScene(CCTransitionFade::create(.5f, SoggifyMenuLayer::scene()));
                })
                .id("sog-btn"_spr)
                .parent(menu);
            menu->updateLayout();
        }
        if (Mod::get()->getSavedValue<bool>("i_1")) {
            sogutils::sog(static_cast<CCSprite*>(this->getChildByID("main-title")));
        }
        // TODO: maybe have it hook CCMenuItemSpriteExtra to be a random button instead
        if (auto menu = typeinfo_cast<CCMenu*>(this->getChildByID("main-menu"))) {
            sogutils::addHidingSog(this, menu);
        }
        return true;
    }
};

$execute {
    listenForSettingChanges("sog-rain-count", [](int value) {
        changedSetting = true;
    });
    listenForSettingChanges("sog-rain-gravity", [](int value) {
        changedSetting = true;
    });
    listenForSettingChanges("sog-rain-scale", [](int value) {
        changedSetting = true;
    });
}
