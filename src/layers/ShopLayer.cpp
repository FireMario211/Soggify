// okay yeah i didnt even try with the security, but who cares!? its not like its online
#include "ShopLayer.hpp"
#include <defs/geode.hpp>
#include "ShopItemLayer.hpp"

// have checkboxes for each shop item, also have them hidden by default until you unlock more. also add an info icon next to the items and maybe make it a scroll layer idk
// ideas:
// sog name (checkbox on by default)
// sog icons
// sog buttons
// jumpscare (adds a button on the bottom left corner of SoggifyMenuLayer that does the redirecting to congregation)
// funny mini game (uhh legend of zelda DT ch3 recreation? maybe do this at the very end if i still have time idk)
// sog background
// sog rain
// SOG EVERYTHING
// gambling! (topest item, will give a random reward, -50 soggy points, 50 soggy points, or mystery stuff not unlockable by shop)
// nuke, will show popup (will play a sound with a video of a nuke, with the text "the soggy mods were the friends we made along the way"), then you cant tap on the sog button anymore as itll play a loud incorrect noise sound effect
//
// gain (player stars / 100) points every 15 seconds

using namespace geode::prelude;

ShopLayer* ShopLayer::create() {
    auto ret = new ShopLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

CCScene* ShopLayer::scene() {
    auto layer = ShopLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

void ShopLayer::sogDance(float) {
    log::info("start sog dance");
    
    // we dont cleanup because why not
    auto randVal = rand() % 10;
    if (randVal % 2 == 0) {
        Build<CCSprite>::create("SOGGY.png"_spr)
            .scale(0.4f)
            .anchorPoint(0.5, 0)
            .runAction(
                CCSequence::create(
                    CCSpawn::createWithTwoActions(
                        CCMoveBy::create(0.5f, ccp(-20, 0)),
                        CCRotateBy::create(0.5f, 25)
                    ),
                    CCSpawn::createWithTwoActions(
                        CCMoveBy::create(0.5f, ccp(-20, 0)),
                        CCRotateBy::create(0.5f, -25 * 2)
                    ),
                    CCSpawn::createWithTwoActions(
                        CCMoveBy::create(0.5f, ccp(-20, 0)),
                        CCRotateBy::create(0.5f, 25 * 2)
                    ),
                    CCSpawn::createWithTwoActions(
                        CCMoveBy::create(0.5f, ccp(-20, 0)),
                        CCRotateBy::create(0.5f, -25)
                    ),
                    CCDelayTime::create(1.0f),
                    CCEaseOut::create(CCMoveBy::create(0.5f, ccp(0, 20)), 3.0f),
                    CCEaseIn::create(CCMoveBy::create(0.5f, ccp(0, -20)), 3.0f),
                    CCDelayTime::create(1.0f),
                    CCMoveBy::create(1.0f, ccp(-80, 0)),
                    CCDelayTime::create(1.0f),
                    CCMoveBy::create(3.0f, ccp(-80, 0)),
                    CCDelayTime::create(0.1f),
                    CCEaseOut::create(CCMoveBy::create(0.5f, ccp(0, 30)), 3.0f),
                    CCEaseIn::create(CCMoveBy::create(0.5f, ccp(0, -90)), 3.0f),
                    nullptr
                )
            )
            .parentAtPos(m_buttonMenu, Anchor::BottomRight, {25, 0})
            .collect();
    } else {
        Build<CCSprite>::create("SOGGY.png"_spr)
            .scale(0.4f)
            .anchorPoint(0.5, 0)
            .runAction(
                CCSequence::createWithTwoActions(
                    CCSpawn::createWithTwoActions(
                        CCMoveBy::create(1.0f, ccp(300, 160)),
                        CCRotateBy::create(1.0f, 90)
                    ),
                    CCSpawn::createWithTwoActions(
                        CCMoveBy::create(1.0f, ccp(320, -80)),
                        CCRotateBy::create(1.0f, 45)
                    )
                )
            )
            .parentAtPos(m_buttonMenu, Anchor::BottomLeft, {-25, 0})
            .collect();
    }
}

void ShopLayer::startLoop2(float) {
    log::info("start loop");
    m_playedLoop = true;
    FMODAudioEngine::sharedEngine()->playMusic("theme2.mp3"_spr, true, 0, 0);
    //this->scheduleOnce(schedule_selector(ShopLayer::sogDance), 1);
    this->scheduleOnce(schedule_selector(ShopLayer::sogDance), (rand() % 100));
}

// Sog Shop Channel
bool ShopLayer::init() {
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

            Build<CCLabelBMFont>::create("SOG", "PGothic.fnt"_spr)
                .anchorPoint(0.5, 1)
                .alignment(CCTextAlignment::kCCTextAlignmentCenter)
                .color({52, 190, 237})
                .store(m_title)
                .parentAtPos(layer, Anchor::Top, {0, -20});
        });
    Build<CCMenu>::create().anchorPoint(0.5,0.5).parent(this).zOrder(1).store(m_buttonMenu);
    Build<CCMenu>::create().parent(this).store(m_fourTitlesMenu).layout(
        ColumnLayout::create()
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setAutoScale(false)
            ->setCrossAxisReverse(true)
            ->setAxisReverse(true)
            ->setCrossAxisAlignment(AxisAlignment::Start)
        ).contentSize({345, 160});
    m_fourTitlesMenu->setPositionY(m_fourTitlesMenu->getPositionY() + 20);

    float baseScale = 1.75f;
    Build<CCSprite>::createSpriteName("colorSpike_02_glow_001.png")
        .scaleX(0.8f * baseScale)
        .scaleY(1.2f * baseScale)
        .color({52, 190, 237})
        .rotation(-90)
        .runAction(
            Build<CCSequence>::create(
                CCEaseOut::create(CCMoveBy::create(0.5f, ccp(-3, 0)), 2.0f),
                CCEaseIn::create(CCMoveBy::create(0.5f, ccp(3, 0)), 2.0f)
            ).repeatForever()
        )
        .intoMenuItem([this]() {
            if (m_currentPage == 0) {
                m_currentPage = sogutils::getCategories().size() - 1;
            } else {
                m_currentPage--;
            }
            updateTitles();
        })
        .parentAtPos(m_buttonMenu, Anchor::Left, {95, 20});
    Build<CCSprite>::createSpriteName("colorSpike_02_glow_001.png")
        .scaleX(0.8f * baseScale)
        .scaleY(1.2f * baseScale)
        .color({52, 190, 237})
        .rotation(90)
        .runAction(
            Build<CCSequence>::create(
                CCEaseOut::create(CCMoveBy::create(0.5f, ccp(3, 0)), 2.0f),
                CCEaseIn::create(CCMoveBy::create(0.5f, ccp(-3, 0)), 2.0f)
            ).repeatForever()
        )
        .intoMenuItem([this]() {
            if (m_currentPage >= sogutils::getCategories().size() - 1) {
                m_currentPage = 0;
            } else {
                m_currentPage++;
            }
            updateTitles();
        })
        .parentAtPos(m_buttonMenu, Anchor::Right, {-95, 20});

    m_buttonMenu->addChildAtPosition(CCMenuItemExt::createSpriteExtra(sogutils::createShopButton("Back"), [this](auto) {
        onClose();
    }), Anchor::Bottom, {-80, 80});
    m_buttonMenu->addChildAtPosition(CCMenuItemExt::createSpriteExtra(sogutils::createShopButton("View All"), [](auto) {
        //CCDirector::get()->pushScene(ViewAllLayer::scene());
        FMODAudioEngine::sharedEngine()->playEffect("INCORRECT.mp3"_spr, 1, 0, 0.5f);
        FLAlertLayer::create("BOO!", "haha i broke the illusion\nalso this is <cy>not implemented yet</c>, please wait until a future update!", "rude")->show();
    }), Anchor::Bottom, {80, 80});


    Build<CCLabelBMFont>::create(fmt::to_string(Mod::get()->getSavedValue<int>("points")).c_str(), "PGothic.fnt"_spr)
        .scale(0.5f)
        .store(m_pointLabel)
        .alignment(CCTextAlignment::kCCTextAlignmentCenter)
        .color(52, 190, 237)
        .parentAtPos(m_buttonMenu, Anchor::Bottom, {0, 35});
    // yeah because i know therell be someone thatll say "1 sog points!"
    Build<CCLabelBMFont>::create(fmt::format("Sog Point{}", Mod::get()->getSavedValue<int>("points") == 1 ? "" : "s").c_str(), "PGothic.fnt"_spr)
        .scale(0.4f)
        .alignment(CCTextAlignment::kCCTextAlignmentCenter)
        .color(40, 40, 40)
        .parentAtPos(m_buttonMenu, Anchor::Bottom, {0, 20});
    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
    FMODAudioEngine::sharedEngine()->playMusic("theme1.mp3"_spr, false, 1, 0);
    this->scheduleOnce(schedule_selector(ShopLayer::startLoop2), 6.5f);
    this->scheduleUpdate();
    updateTitles();
    return true;
}

void ShopLayer::updateTitles() {
    m_fourTitlesMenu->removeAllChildrenWithCleanup(true);
    auto category = sogutils::getCategories()[m_currentPage];
    std::vector<ShopItem> filteredItems;
    for (auto item : sogutils::getItems()) {
        if (item.categoryId == m_currentPage + 1) {
            filteredItems.push_back(item);
        }
    }
    m_title->setString(category.name.c_str());
    for (uint8_t i = 0; i < 4; i++) {
        if (i >= filteredItems.size()) {
            //Build<CCScale9Sprite>::create("square02b_small.png")
                //.contentSize({170, 70})
                //.parent(m_fourTitlesMenu);
        } else {
            auto item = filteredItems[i];
            Build<CCScale9Sprite>::create("square02b_small.png")
                .contentSize({170, 70})
                .with([item](auto spr) {
                    auto squareSpr = Build<CCScale9Sprite>::create("square.png")
                        .anchorPoint(0, 0.5)
                        .scale(0.8f)
                        .contentSize({70, 45})
                        .color({0,0,0})
                        .parentAtPos(spr, Anchor::Left, {10, 0})
                        .collect();
                    Build<CCNode>::create()
                        .zOrder(-1)
                        .with([](auto node) {
                            Build<CCSprite>::createSpriteName("d_gradient_02_001.png").color(0,0,0).pos(.25,.25).anchorPoint(1,0).parent(node);
                            Build<CCSprite>::createSpriteName("d_gradient_02_001.png").color(0,0,0).pos(.25,.25).scaleX(-1).scaleY(1).anchorPoint(1,0).parent(node);
                            Build<CCSprite>::createSpriteName("d_gradient_02_001.png").color(0,0,0).pos(.25,.25).scaleX(1).scaleY(-1).anchorPoint(1,0).parent(node);
                            Build<CCSprite>::createSpriteName("d_gradient_02_001.png").color(0,0,0).pos(.25,.25).scaleX(-1).scaleY(-1).anchorPoint(1,0).parent(node);
                        })
                        .scaleY(1.8f)
                        .scaleX(2.5f)
                        .parentAtPos(squareSpr, Anchor::Center);
                    bool boughtItem = Mod::get()->hasSavedValue(fmt::format("i_{}", item.id));
                    Build<CCSprite>::create(fmt::format("preview{}.png"_spr, item.id).c_str())
                        .scaleToMatchX(squareSpr->getContentWidth())
                        .scaleToMatchY(squareSpr->getContentHeight())
                        .opacity((boughtItem) ? 100 : 255)
                        .parentAtPos(squareSpr, Anchor::Center);
                    if (boughtItem) {
                        Build<CCSprite>::createSpriteName(
                            (Mod::get()->getSavedValue<bool>(fmt::format("i_{}", item.id))) ? "GJ_completesIcon_001.png" : "GJ_deleteIcon_001.png"
                        ).parentAtPos(squareSpr, Anchor::Center);
                    }
                    Build<CCLabelBMFont>::create(item.name.c_str(), "PGothic.fnt"_spr)
                        .anchorPoint(0, 0)
                        .scale(0.4f)
                        .alignment(CCTextAlignment::kCCTextAlignmentLeft)
                        .color({40, 40, 40})
                        .parentAtPos(spr, Anchor::Center, {-15, 5});
                    Build<CCLabelBMFont>::create(fmt::format("{} Sog Points", item.price).c_str(), "PGothic.fnt"_spr)
                        .anchorPoint(0, 0)
                        .scale(0.35f)
                        .alignment(CCTextAlignment::kCCTextAlignmentLeft)
                        .color({40, 40, 40})
                        .parentAtPos(spr, Anchor::Center, {-15, -20});
                })
                .intoMenuItem([item, this]() {
                    if (item.id == 9 && Mod::get()->hasSavedValue("nuke02")) {
                        FMODAudioEngine::sharedEngine()->playEffect("INCORRECT.mp3"_spr, 1, 0, GameManager::get()->m_sfxVolume);
                        return;
                    }
                    if (!m_playedLoop) {
                        m_playedLoop = true;
                        startLoop2(0.f);
                    }
                    CCDirector::get()->pushScene(ShopItemLayer::scene(item));
                })
                .parent(m_fourTitlesMenu);
        }
    }
    m_fourTitlesMenu->updateLayout();
}

void ShopLayer::onEnter() {
    CCLayer::onEnter();
    updateTitles();
    m_pointLabel->setString(fmt::to_string(Mod::get()->getSavedValue<int>("points")).c_str());
}

void ShopLayer::update(float dt) {
    CCLayer::update(dt);
    CCPoint pos = getMousePos();
    if (pos != m_lastMousePos) {
        m_lastMousePos = pos;
        if (m_fourTitlesMenu->getChildrenCount() == 0) return;
        for (int i = 0; i < m_fourTitlesMenu->getChildrenCount(); i++) {
            if (auto node = typeinfo_cast<CCMenuItemSpriteExtra*>(m_fourTitlesMenu->getChildren()->objectAtIndex(i))) {
                if (node->boundingBox().containsPoint(m_fourTitlesMenu->convertToNodeSpace(pos))) {
                    static_cast<CCScale9Sprite*>(node->getChildren()->objectAtIndex(0))->setColor({200, 240, 250});
                } else {
                    static_cast<CCScale9Sprite*>(node->getChildren()->objectAtIndex(0))->setColor({255,255,255});
                }
            }
        }
    }
}

void ShopLayer::onClose() {
    if ((rand() % 40) < 20) {
        GameManager::sharedState()->fadeInMusic("capeling.soggy-mod/SogLoop.mp3");
    } else {
        GameManager::sharedState()->fadeInMusic("capeling.soggy-mod/SogLoop02.mp3");
    }
    CCDirector::get()->popSceneWithTransition(0.5F, PopTransition::kPopTransitionFade);
    //CCDirector::get()->replaceScene(CCTransitionFade::create(.5f, SoggifyMenuLayer::scene()));
}
