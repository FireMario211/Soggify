#include "SpinWheelGame.hpp"
#include <defs/geode.hpp>
#include <layers/SoggifyMenuLayer.hpp>

void SpinWheelGame::setRewards() {
    m_rewards.clear();
    m_wheelColors.clear();
    bool has_loss = false;
    for (int i = 0; i < m_numSectors; ++i) {
        int type = rand() % 100;
        int reward = 0;
        ccColor4F color;
        if (type < 5) {
            // GOLD GOLD GOLD
            reward = 30 + rand() % 61; // 30-90% win
            color = { 1.0f, 0.84f, 0.0f, 1.0f };
        } else if (type < 50) {
            reward = 0;
            color = { 0.0f, 0.0f, 0.0f, 0.4f };
        } else if (type < 60) {
            reward = - (10 + rand() % 26); // 10-35% loss
            color = { 1.0f, 0.3f, 0.3f, 1.0f };
            has_loss = true;
        } else {
            reward = 5 + rand() % 11; // 5-15% win
            color = { 0.3f, 1.0f, 0.3f, 1.0f };
        }
        m_rewards.push_back(reward);
        m_wheelColors.push_back(color);
    }
    if (!has_loss) {
        int randomIndex = rand() % (m_numSectors - 1);
        m_rewards[randomIndex] = - (10 + rand() % 26);
        m_wheelColors[randomIndex] = { 1.0f, 0.3f, 0.3f, 1.0f };
        log::debug("Didn't have loss, adding one.");
    }
    if (m_wheel) {
        m_wheel->setColors(m_wheelColors);
    }
}

bool SpinWheelGame::setup() {
    Loader::get()->queueInMainThread([](){
        FLAlertLayer::create("How to", "Tap the <cg>Spin</c> button and <cy>gain</c> or <cr>lose</c> <cy>Sog Points</c>!", "OK")->show();
    });
    setRewards();
    Build<CCNode>::create().store(m_container).with([this](auto node) {
        m_wheel = WheelNode::create(m_numSectors, m_wheelColors);
        node->addChild(m_wheel);
        Build<CCSprite>::create("circle.png"_spr).zOrder(3).parent(node);
        for (int i = 0; i < m_numSectors; ++i) {
            auto ln = BreakLine::create(2.0f, 100.0f * 2, { 0, 0, 0, 1 });
            ln->setAnchorPoint({0.5, 0.5});
            ln->setPosition(ccp(0, 0));
            ln->setRotation(i * (360.0f / m_numSectors));
            node->addChild(ln, 2);
        }
    }).parentAtPos(m_mainLayer, Anchor::Center, {0, -10});
    m_mainLayer->addChildAtPosition(BreakLine::create(2.0f, 100.0f, { 1, 1, 1, 1 }), Anchor::Center, {-1, -10});
    Build<ButtonSprite>::create("Spin!", "bigFont.fnt", "GJ_button_01.png").scale(0.85f).intoMenuItem([this]() {
        if (m_isSpinning) return;
        LETSGOGAMBLING();
    }).parentAtPos(m_buttonMenu, Anchor::Bottom, {0, -10});

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
    dict->setObject(CCString::create(std::to_string(20)), "startParticleSize");
    dict->setObject(CCString::create(std::to_string(10)), "finishParticleSize");
    dict->setObject(CCString::create("3"), "startParticleSizeVariance");
    dict->setObject(CCString::create("0"), "speed");
    dict->setObject(CCString::create("0"), "speedVariance");
    dict->setObject(CCString::create("1"), "gravityx");
    dict->setObject(CCString::create(std::to_string(-600)), "gravityy");
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

    m_emitter = CCParticleSystemQuad::create();
    m_emitter->initWithDictionary(dict, false);
    m_emitter->setTexture(CCSprite::create("SOGGY.png"_spr)->getTexture());
    auto winSize = CCDirector::get()->getWinSize();
    m_emitter->setZOrder(CCScene::get()->getHighestChildZ() + 1);
    m_emitter->setPosition({winSize.width / 2, winSize.height + 20});
    m_emitter->setVisible(false);
    this->addChild(m_emitter);
    scheduleUpdate();
    return true;
}

void SpinWheelGame::update(float dt) {
    CCLayer::update(dt);
    if (!m_isSpinning) {
        float rot = m_container->getRotation() + 10.f * dt;
        m_container->setRotation(fmodf(rot, 360.0f));
    } else { // what is that ticking noise...
        float currAngle = fmodf(m_container->getRotation(), 360.0f);
        float sectorSize = 360.0f / m_rewards.size();
        int prevSector = (int)(m_lastAngle / sectorSize);
        int currSector = (int)(currAngle / sectorSize);
        if (currSector != prevSector) {
            FMODAudioEngine::sharedEngine()->playEffect("s447.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume);
        }
        m_lastAngle = currAngle;
    }
}

void SpinWheelGame::LETSGOGAMBLING() {
    if (m_isSpinning) return;
    m_isSpinning = true;
    m_emitter->setVisible(false);
    FMODAudioEngine::sharedEngine()->playEffect("gamblecore.mp3"_spr, 1, 0, GameManager::get()->m_sfxVolume);
    // this was hard to do i didnt even do it right
    m_container->runAction(CCSequence::create(
        CCEaseSineOut::create(CCRotateBy::create(1.0f, -90 + (rand() % 90))),
        CCEaseSineIn::create(CCRotateBy::create(1.0f, 360 * 2)),
        CCRotateBy::create(2.0f, 360 * 3.75f),
        CCEaseSineOut::create(CCRotateBy::create(2.5f, 720 + CCRANDOM_0_1() * 360.0f)),
        CCEaseSineOut::create(CCRotateBy::create(3.0f, 45.0f + CCRANDOM_0_1() * 45.0f)),
        CCDelayTime::create(1.0f),
        CCCallFunc::create(this, callfunc_selector(SpinWheelGame::onSpinComplete)),
        nullptr
    ));
}

// math is mathing math mathing
// so i thought i could just normalize the rotation [0, 360), and do mod(R,8) to get index right? NO!
// i dont know why but it was always off, this was so annoying to fix
// well idk if it works properly or the calculations are right but so far they seem to be... lets just ship this to prod and hope no one complains!
void SpinWheelGame::onSpinComplete() {
    m_isSpinning = false;
    float rot = fmodf(m_container->getRotation(), 360.0f);
    if (rot < 0) rot += 360.f;
    float combined = 90.f + rot;
    if (combined < 0.f) combined += 360.f;
    float sectorSize = 360.0f / (float)m_rewards.size();
    const float eps = 1e-6f;
    int sector = static_cast<int>(floorf((combined + eps) / sectorSize)) % m_rewards.size();
    if (sector < 0) sector += m_rewards.size();
    int rewardPercent = m_rewards[sector];
    int reward = (int)std::llround(Mod::get()->getSavedValue<int>("points", 0) * rewardPercent / 100.0);
    log::debug("wheelRotRaw={} rot={} combined={} sectSize={} sect={}", m_container->getRotation(), rot, combined, sectorSize, sector);
    if (reward > 0) { // YOU WON
        if (rewardPercent >= 30) { // GOLD GOLD GOLD
            FLAlertLayer::create("GOLD GOLD GOLD", fmt::format("YOU WON <cy>{} SOG POINTS!!!</c>", reward).c_str(), "yay")->show();
            FMODAudioEngine::sharedEngine()->playEffect("highscoreGet02.ogg", 1, 0, GameManager::get()->m_sfxVolume);
            FMODAudioEngine::sharedEngine()->playEffect("gold01.ogg", 1, 0, GameManager::get()->m_sfxVolume);
            m_emitter->setVisible(true);
            m_emitter->resetSystem();
            m_emitter->update(1.f/240.f);
            m_emitter->update(1.f/240.f);
            m_emitter->update(1.f/240.f);
            m_emitter->update(1.f/240.f);
            m_emitter->update(1.f/240.f);
        } else {
            FLAlertLayer::create("YOU WON!!!", fmt::format("You <cy>won +{} Sog Points!!!</c>", reward).c_str(), "yay")->show();
            FMODAudioEngine::sharedEngine()->playEffect("reward01.ogg", 1, 0, GameManager::get()->m_sfxVolume);
            FMODAudioEngine::sharedEngine()->playEffect("gold01.ogg", 1, 0, GameManager::get()->m_sfxVolume);
        }
    } else if (reward < 0) { // YOU LOST
        FLAlertLayer::create("you lost", fmt::format("You <cr>lost {} Sog Points</c>, rip", reward).c_str(), "really")->show();
        FMODAudioEngine::sharedEngine()->playEffect("INCORRECT.mp3"_spr, 1, 0, GameManager::get()->m_sfxVolume);
    } else {
        FLAlertLayer::create("you won nothing", "spin again!!!", "OK")->show();
    }
    Mod::get()->setSavedValue("points", Mod::get()->getSavedValue<int>("points", 0) + reward);
    if (auto menu = static_cast<SoggifyMenuLayer*>(this->getParent()->getChildByID("SoggifyMenuLayer"_spr))) {
        menu->updatePoints();
    }
    setRewards();
}
