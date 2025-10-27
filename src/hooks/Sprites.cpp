#include <Geode/Geode.hpp>
#include <defs/utils.hpp>
#include <Geode/modify/CCSprite.hpp>
#include <Geode/modify/CCScale9Sprite.hpp>
#include <Geode/modify/CCMenuItemSpriteExtra.hpp>

using namespace geode::prelude;
class $modify(SoggifyCCSprite, CCSprite) {
    static CCSprite* create(char const* name) {
        auto ret = CCSprite::create(name);
        if (ret == nullptr) return ret;
        if (Mod::get()->getSettingValue<bool>("ery-mode")) return ret;
        if (std::string(name).starts_with("firee.soggify")) return ret;
        if (std::string(name).starts_with("GJ_gradient")) {
            // probably a bad idea but whatevs!
            Loader::get()->queueInMainThread([ret]() {
                sogutils::sog(ret, {0, 0}, ret->getColor());
            });
            return ret;
        }
        return ret;
        if (name == "SOGGY.png"_spr) return ret;
        if (!Mod::get()->hasSavedValue("i_6") || !Mod::get()->getSavedValue<bool>("i_6")) return ret;
        auto sog = CCSprite::create("SOGGY.png"_spr);
        sog->setScaleX(ret->getContentWidth() / sog->getContentWidth());
        sog->setScaleY(ret->getContentHeight() / sog->getContentHeight());
        //sog->setOpacity(100);
        sog->setAnchorPoint({0, 0});
        ret->addChild(sog);
        return ret;
    }
    /*static CCSprite* createWithSpriteFrameName(char const* name) {
        auto ret = CCSprite::createWithSpriteFrameName(name);
        if (ret == nullptr) return ret;
        return ret;
        if (std::string(name).starts_with("firee.soggify")) return ret;
        if (!Mod::get()->hasSavedValue("i_6") || !Mod::get()->getSavedValue<bool>("i_6")) return ret;
        auto sog = CCSprite::create("SOGGY.png"_spr);
        sog->setScaleX(ret->getContentWidth() / sog->getContentWidth());
        sog->setScaleY(ret->getContentHeight() / sog->getContentHeight());
        //sog->setOpacity(100);
        sog->setAnchorPoint({0, 0});
        ret->addChild(sog);
        return ret;
    }*/
};
/*
class $modify(SoggifyCCScale9Sprite, CCScale9Sprite) {
    void setContentSize(const CCSize &size) {
        CCScale9Sprite::setContentSize(size);
        bool enableSogEverything = Mod::get()->getSavedValue<bool>("i_6");
        if (!enableSogEverything) return;
        auto sog = CCSprite::create("SOGGY.png"_spr);
        sog->setScaleX(size.width / sog->getContentWidth());
        sog->setScaleY(size.height / sog->getContentHeight());
        this->setOpacity(100);
        sog->setAnchorPoint({0, 0});
        this->addChild(sog);
    }
};
*/
class $modify(SoggifyCCMenuItem, CCMenuItemSpriteExtra) {
    static CCMenuItemSpriteExtra* create(cocos2d::CCNode* sprite, cocos2d::CCNode* disabledSprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback) {
        if (sprite == nullptr) return CCMenuItemSpriteExtra::create(sprite, disabledSprite, target, callback);
        if (Mod::get()->getSettingValue<bool>("ery-mode")) return CCMenuItemSpriteExtra::create(sprite, disabledSprite, target, callback);
        if (!Mod::get()->getSavedValue<bool>("i_6") && !Mod::get()->getSavedValue<bool>("i_3")) return CCMenuItemSpriteExtra::create(sprite, disabledSprite, target, callback);
        auto sog = CCSprite::create("SOGGY.png"_spr);
        sog->setScaleX(sprite->getContentWidth() / sog->getContentWidth());
        sog->setScaleY(sprite->getContentHeight() / sog->getContentHeight());
        sog->setAnchorPoint({0, 0});
        sog->setOpacity(222); // just so no one actually complains...
        sprite->addChild(sog);
        return CCMenuItemSpriteExtra::create(sprite, disabledSprite, target, callback);
    }
};

// $on_mod(Loaded) {
//     bool enableSogButtons = Mod::get()->getSavedValue<bool>("i_3");
//     bool enableSogEverything = Mod::get()->getSavedValue<bool>("i_6");
//     sogutils::toggleLayerHook("CCMenuItemSpriteExtra::create", enableSogButtons || enableSogEverything);
//     sogutils::toggleLayerHook("cocos2d::CCScale9Sprite::setContentSize", enableSogEverything);
//     sogutils::toggleLayerHook("cocos2d::CCSprite::create", enableSogEverything);
//     sogutils::toggleLayerHook("cocos2d::CCSprite::createWithSpriteFrameName", enableSogEverything);
// }
//
// $execute {
//     bool enableSogButtons = Mod::get()->getSavedValue<bool>("i_3");
//     bool enableSogEverything = Mod::get()->getSavedValue<bool>("i_6");
//     sogutils::toggleLayerHook("CCMenuItemSpriteExtra::create", enableSogButtons || enableSogEverything);
//     sogutils::toggleLayerHook("cocos2d::CCScale9Sprite::setContentSize", enableSogEverything);
//     sogutils::toggleLayerHook("cocos2d::CCSprite::create", enableSogEverything);
//     sogutils::toggleLayerHook("cocos2d::CCSprite::createWithSpriteFrameName", enableSogEverything);
// };
$on_mod(Loaded) {
    bool enabled1 = Mod::get()->hasSavedValue("i_3") && Mod::get()->getSavedValue<bool>("i_3");
    bool enabled2 = Mod::get()->hasSavedValue("i_4") && Mod::get()->getSavedValue<bool>("i_4");
    sogutils::toggleLayerHook("CCMenuItemSpriteExtra::create", enabled1);
    sogutils::toggleLayerHook("cocos2d::CCScale9Sprite::setContentSize", false);
    sogutils::toggleLayerHook("cocos2d::CCSprite::create", enabled2);
    sogutils::toggleLayerHook("cocos2d::CCSprite::createWithSpriteFrameName", false);
}
