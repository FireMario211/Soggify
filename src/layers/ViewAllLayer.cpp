#include "ViewAllLayer.hpp"
#include <defs/geode.hpp>
#include <defs/utils.hpp>

using namespace geode::prelude;

ViewAllLayer* ViewAllLayer::create() {
    auto ret = new ViewAllLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

CCScene* ViewAllLayer::scene() {
    auto layer = ViewAllLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

bool ViewAllLayer::init() {
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
            Build<CCLabelBMFont>::create("All Items", "PGothic.fnt"_spr)
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

void ViewAllLayer::onEnter() {
    CCLayer::onEnter();
    m_pointLabel->setString(fmt::to_string(Mod::get()->getSavedValue<int>("points")).c_str());
}

void ViewAllLayer::onClose() {
    CCDirector::get()->popSceneWithTransition(0.5F, PopTransition::kPopTransitionFade);
}
