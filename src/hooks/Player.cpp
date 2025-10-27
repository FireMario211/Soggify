#include <Geode/Geode.hpp>
#include <defs/utils.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/SimplePlayer.hpp>

using namespace geode::prelude;

class $modify(PlayerObject) {
    bool init(int player, int ship, GJBaseGameLayer *gameLayer, cocos2d::CCLayer *layer, bool playLayer) {
        if (!PlayerObject::init(player,ship,gameLayer,layer,playLayer)) return false;
        if (Mod::get()->getSettingValue<bool>("ery-mode")) return true;
        sogutils::sog(m_iconSprite, {0, 0}, m_iconSprite->getColor());
        sogutils::sog(m_iconSpriteSecondary, {0, 0}, m_iconSprite->getColor());
        return true;
    }
    void updatePlayerFrame(int p0) {
        PlayerObject::updatePlayerFrame(p0);
        if (Mod::get()->getSettingValue<bool>("ery-mode")) return;
        sogutils::sog(m_iconSprite, {0, 0}, m_iconSprite->getColor());
        sogutils::sog(m_iconSpriteSecondary, {0, 0}, m_iconSprite->getColor());
    }
};

class $modify(SimplePlayer) {
    void updatePlayerFrame(int p0, IconType p1) {
        SimplePlayer::updatePlayerFrame(p0,p1);
        if (Mod::get()->getSettingValue<bool>("ery-mode")) return;
        sogutils::sog(m_firstLayer, {0, 0}, m_firstLayer->getColor());
        sogutils::sog(m_secondLayer, {0, 0}, m_firstLayer->getColor());
        //if (p1 == IconType::Ufo) sogutils::sog(m_birdDome, {0, 0}, m_firstLayer->getColor());
        sogutils::sog(m_outlineSprite, {0, 0}, m_firstLayer->getColor());
        //sogutils::sog(m_detailSprite);
        if (p1 == IconType::Robot) sogutils::sog(m_robotSprite, {0, 0}, m_firstLayer->getColor());
        if (p1 == IconType::Spider) sogutils::sog(m_spiderSprite, {0, 0}, m_firstLayer->getColor());
    }
};

$on_mod(Loaded) {
    bool enabled = false;
    if (Mod::get()->hasSavedValue("i_2") && Mod::get()->getSavedValue<bool>("i_2")) {
        enabled = true;
    }
    sogutils::toggleLayerHook("PlayerObject::init", enabled);
    sogutils::toggleLayerHook("PlayerObject::updatePlayerFrame", enabled);
    sogutils::toggleLayerHook("SimplePlayer::updatePlayerFrame", enabled);
}
