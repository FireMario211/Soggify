// this is partly the reason i didnt add mobile support
#include "Game.hpp"
#include <defs/geode.hpp>

bool Game::initGame(std::string name) {
    /*if (!CCLayer::init()) return false;
    auto director = cocos2d::CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    this->m_name = name;
    setup();
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
    this->setKeyboardEnabled(true);
    return true;*/
    m_name = name;
    if (this->initAnchored(380, 260)) {
        this->autorelease();
        this->setTitle(name);
        Build<CCSprite>::createSpriteName("dailyLevelCorner_001.png").anchorPoint(0,1).flipY(true).parentAtPos(m_mainLayer, Anchor::TopLeft);
        Build<CCSprite>::createSpriteName("dailyLevelCorner_001.png").anchorPoint(1,1).flipX(true).flipY(true).parentAtPos(m_mainLayer, Anchor::TopRight);
        Build<CCSprite>::createSpriteName("dailyLevelCorner_001.png").anchorPoint(1,0).flipX(true).parentAtPos(m_mainLayer, Anchor::BottomRight);
        Build<CCSprite>::createSpriteName("dailyLevelCorner_001.png").anchorPoint(0,0).parentAtPos(m_mainLayer, Anchor::BottomLeft);
        return true;
    }
    return false;
}
