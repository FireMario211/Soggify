#include <defs/geode.hpp>
#include <defs/utils.hpp>
#include <Geode/modify/LoadingLayer.hpp>

class $modify(SoggifyHookLL, LoadingLayer) {
    bool init(bool refresh) {
        if (!LoadingLayer::init(refresh)) return false;
        if (Mod::get()->getSettingValue<bool>("ery-mode")) return true;
        if (Mod::get()->getSavedValue<bool>("i_1")) {
            Loader::get()->queueInMainThread([this]() {
                if (auto node = static_cast<CCSprite*>(getChildByID("gd-logo"))) {
                    sogutils::sog(node);
                }
            });
        }
        return true;
    }
};

$execute {
    if (Loader::get()->isPatchless()) {
        sogutils::toggleLayerHook("LoadingLayer::init", false);
    }
}

$on_mod(Loaded) {
    if (Loader::get()->isPatchless()) {
        sogutils::toggleLayerHook("LoadingLayer::init", false);
    }
    if (Mod::get()->hasSavedValue("quests")) {
        Mod::get()->getSaveContainer().erase("quests");
    }
    if (!Mod::get()->hasSavedValue("i_1")) {
        Mod::get()->setSavedValue("i_1", true);
    }
}
