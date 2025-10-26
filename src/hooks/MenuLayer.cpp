#include <defs/geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <layers/SoggifyMenuLayer.hpp>
#include <defs/utils.hpp>

CCParticleSystemQuad* s_emitter = nullptr;

class $modify(SoggifyHookML, MenuLayer) {
    static void onModify(auto& self) {
        (void)self.setHookPriority("MenuLayer::init", Priority::First);
    }
    void createParticle(bool enable) {
        if (s_emitter) {
            s_emitter->setVisible(enable);
            return;
        }
        CCPoint sizeParticle = {50,60};
        int amountParticles = 200;
        //CCParticleSystemQuad* particle_info = GameToolbox::particleFromString(fmt::format("1,2065,2,13050,3,1018.38,155,1,156,2,145,200a-1a1a0.3a200a-90a90a0a0a100a0a1a-1000a0a0a0a0a20a3a0a0a1a0a0.686275a0a0.980392a0a1a0a5a0a0a0a0.0156863a0a0.156863a0a0.392157a0a1a0a0a0a0a0a0a0a0a0a0a0a0a0a1a0a0a0a31a0a0a0a0a0a0a0a0a0a0a0a0a0a0;", sizeParticle.x, sizeParticle.y, amountParticles), NULL, false);
        //particle_info->setTexture(CCSprite::create("SOGGY.png"_spr)->getTexture());
        //particle_info->setZOrder(-100);
        auto dict = CCDictionary::createWithContentsOfFileThreadSafe("dragEffect.plist");

        dict->setObject(CCString::create("0"), "emitterType");
        dict->setObject(CCString::create("-1"), "duration");
        dict->setObject(CCString::createWithFormat("%i", 200), "maxParticles");
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
        dict->setObject(CCString::create("20"), "startParticleSize");
        dict->setObject(CCString::create("10"), "finishParticleSize");
        dict->setObject(CCString::create("3"), "startParticleSizeVariance");
        dict->setObject(CCString::create("0"), "speed");
        dict->setObject(CCString::create("0"), "speedVariance");
        dict->setObject(CCString::create("1"), "gravityx");
        dict->setObject(CCString::create("-600"), "gravityy");
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
    /*
        dict->setObject(CCString::create("1"), "emitterType");
        dict->setObject(CCString::create("-1"), "duration");
        dict->setObject(CCString::createWithFormat("%i", 200), "maxParticles");
        dict->setObject(CCString::create("1.5"), "particleLifespan");
        dict->setObject(CCString::create("0"), "angle");
        dict->setObject(CCString::create("180"), "angleVariance");
        dict->setObject(CCString::create("10"), "maxRadius");
        dict->setObject(CCString::create("30"), "minRadius");
        dict->setObject(CCString::create("0"), "sourcePositionx");
        dict->setObject(CCString::create("0"), "sourcePositiony");
        dict->setObject(CCString::create("0"), "sourcePositionVariancex");
        dict->setObject(CCString::create("0"), "sourcePositionVariancey");
        dict->setObject(CCString::create("4"), "startParticleSize");
        dict->setObject(CCString::create("4"), "finishParticleSize");
        dict->setObject(CCString::create("0"), "startParticleSizeVariance");
        dict->setObject(CCString::create("1"), "startColorRed");
        dict->setObject(CCString::create("1"), "startColorGreen");
        dict->setObject(CCString::create("1"), "startColorBlue");
        dict->setObject(CCString::create("1"), "startColorAlpha");
        dict->setObject(CCString::create("1"), "startColorVarianceRed");
        dict->setObject(CCString::create("1"), "startColorVarianceBlue");
        dict->setObject(CCString::create("1"), "startColorVarianceGreen");
        dict->setObject(CCString::create("0"), "startColorVarianceAlpha");
        dict->setObject(CCString::create("1"), "finishColorRed");
        dict->setObject(CCString::create("1"), "finishColorGreen");
        dict->setObject(CCString::create("1"), "finishColorBlue");
        dict->setObject(CCString::create("0"), "finishColorAlpha");
        dict->setObject(CCString::create("1"), "finishColorVarianceRed");
        dict->setObject(CCString::create("1"), "finishColorVarianceBlue");
        dict->setObject(CCString::create("1"), "finishColorVarianceGreen");
        dict->setObject(CCString::create("0"), "finishColorVarianceAlpha");
        dict->setObject(CCString::create("square.png"), "textureFileName");*/

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
