#include "PongGame.hpp"
#include "Geode/ui/BreakLine.hpp"
#include <defs/utils.hpp>
#include <layers/SoggifyMenuLayer.hpp>

// https://t3.ftcdn.net/jpg/06/11/20/08/360_F_611200865_oyq4lBwsNwk7xwA8J6OKF9P3xA6oUkso.jpg

bool PongGame::setup() {
    Loader::get()->queueInMainThread([](){
        FLAlertLayer::create("How to", "Hold <cg>left arrow</c> or <cg>right arrow</c> to move!\nEach time you beat sog, you gain <cy>+10 Sog Points</c>!", "OK")->show();
    });
    Build<CCScale9Sprite>::create("square02b_small.png")
        .contentSize({225.f, 205.f})
        .parentAtPos(m_mainLayer, Anchor::Center, {0, -10});
    Build<CCScale9Sprite>::create("square02b_small.png")
        .color({80, 220, 100})
        .contentSize({220.f, 200.f})
        .parentAtPos(m_mainLayer, Anchor::Center, {0, -10})
        .store(m_bg);

    m_playSize = m_bg->getContentSize();

    Build<CCSprite>::create("SOGGY.png"_spr).scale(0.3f).children(Build<CCSprite>::create("paddle.png"_spr).anchorPoint(0.5, 0).pos(90, 15)).parentAtPos(m_bg, Anchor::Top, {0, -13}).store(m_paddleTop);

    Build<CCScale9Sprite>::create("square.png")
        .contentSize({183.f, 143.f})
        .parentAtPos(m_bg, Anchor::Center);
    Build<CCScale9Sprite>::create("square.png")
        .color({41, 64, 134})
        .contentSize({180.f, 140.f})
        .parentAtPos(m_bg, Anchor::Center);

    //228, 47, 66
    //232, 83, 103
    auto line = BreakLine::create(180, 2.f);
    line->setAnchorPoint({0.5, 0.5});

    auto line2 = BreakLine::create(65, 2.f, {1.f, 1.f, 1.f, 1.f});
    line2->setAnchorPoint({1, 0.5});
    line2->setRotation(90);
    auto line3 = BreakLine::create(65, 2.f, {1.f, 1.f, 1.f, 1.f});
    line3->setAnchorPoint({0, 0.5});
    line3->setRotation(90);

    m_bg->addChildAtPosition(line, Anchor::Center);
    m_bg->addChildAtPosition(line2, Anchor::Center, {0, 5});
    m_bg->addChildAtPosition(line3, Anchor::Center, {0, -5});

    Build<CCSprite>::create("circle.png"_spr).parentAtPos(m_bg, Anchor::Center).store(m_ball);

    auto gm = GameManager::sharedState();
    if (!gm) return false;
    m_paddleBottom = SimplePlayer::create(gm->getPlayerFrame());
    m_paddleBottom->setColor(gm->colorForIdx(gm->getPlayerColor()));
    m_paddleBottom->setSecondColor(gm->colorForIdx(gm->getPlayerColor2()));
    m_paddleBottom->setGlowOutline(gm->colorForIdx(gm->getPlayerGlowColor()));
    m_paddleBottom->m_hasGlowOutline = gm->getPlayerGlow();
    m_paddleBottom->updateColors();
    m_paddleBottom->addChild(Build<CCSprite>::create("paddle.png"_spr).flipX(true).scale(0.4f).anchorPoint(0.5, 0).pos(-17, 0).collect());
    m_paddleBottom->setScale(0.8f);
    m_bg->addChildAtPosition(m_paddleBottom, Anchor::Bottom, {0, 12});
    resetBall(true);

    Build<CCLabelBMFont>::create("0", "goldFont.fnt").alignment(CCTextAlignment::kCCTextAlignmentCenter).parentAtPos(m_bg, Anchor::Left, {-40, 0}).store(m_playerScoreLbl);
    Build<CCLabelBMFont>::create("You", "bigFont.fnt").scale(0.8f).alignment(CCTextAlignment::kCCTextAlignmentCenter).parentAtPos(m_bg, Anchor::Left, {-40, -30});

    Build<CCLabelBMFont>::create("0", "goldFont.fnt").alignment(CCTextAlignment::kCCTextAlignmentCenter).parentAtPos(m_bg, Anchor::Right, {40, 0}).store(m_opponentScoreLbl);
    Build<CCLabelBMFont>::create("Sog", "bigFont.fnt").scale(0.8f).alignment(CCTextAlignment::kCCTextAlignmentCenter).parentAtPos(m_bg, Anchor::Right, {40, -30});

#ifndef GEODE_IS_DESKTOP
    Build<CCSprite>::createSpriteName("PBtn_Move_001.png").store(m_leftBtn).parentAtPos(m_bg, Anchor::Left, {-30, -75});
    Build<CCSprite>::createSpriteName("PBtn_Move_001.png").flipX(true).store(m_rightBtn).parentAtPos(m_bg, Anchor::Right, {30, -75});
#endif

    this->setTouchMode(ccTouchesMode::kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    return true;
}
#ifndef GEODE_IS_DESKTOP
bool PongGame::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *p1) {
    if (!Game::ccTouchBegan(touch,p1)) return false;
    if (!(m_leftBtn && m_rightBtn)) return false;
    auto touchPos = cocos2d::CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
    auto nodeTouchPos = m_bg->convertToNodeSpace(touchPos);
    if (m_leftBtn->boundingBox().containsPoint(nodeTouchPos)) {
        m_leftBtn->setOpacity(150);
        if (!m_started) {
            scheduleUpdate();
            m_started = true;
        }
        m_holdLeft = true;
        m_holdRight = false;
        return true;
    } else if (m_rightBtn->boundingBox().containsPoint(nodeTouchPos)) {
        m_rightBtn->setOpacity(150);
        if (!m_started) {
            scheduleUpdate();
            m_started = true;
        }
        m_holdRight = true;
        m_holdLeft = false;
        return true;
    } else {
        return false;
    }
}

void PongGame::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *p1) {
    Game::ccTouchEnded(touch, p1);
    if (!(m_leftBtn && m_rightBtn)) return;
    m_holdLeft = false;
    m_holdRight = false;
    m_leftBtn->setOpacity(255);
    m_rightBtn->setOpacity(255);
}
#endif
void PongGame::resetBall(bool serveToPlayer) {
    m_ball->updateAnchoredPosition(Anchor::Center);
    m_ballSpeed = m_baseBallSpeed;
    float angleDeg = ((rand() % 2 == 1) ? -30 : 30);
    float angleRad = CC_DEGREES_TO_RADIANS(angleDeg);
    float dirY = serveToPlayer ? -1.0f : 1.0f;
    m_ballVel.x = m_ballSpeed * sinf(angleRad);
    m_ballVel.y = m_ballSpeed * dirY * cosf(angleRad);
    m_timeSinceLastScore = 0.0f;
}

// https://github.com/ItzKiba/GrandpaDemon/blob/1e2853a8f79e7a49fef22765268da83a85cf0bc8/src/EffectsManager.h
void PongGame::reflectOffPaddle(CCNode* paddle) {
    FMODAudioEngine::sharedEngine()->playEffect("s886.ogg"_spr, 1, 0, GameManager::get()->m_sfxVolume);
    float paddleX = paddle->getPositionX();
    float rel = (m_ball->getPositionX() - paddleX) / (m_paddleWidth * 0.5f);
    rel = std::max(-1.0f, std::min(1.0f, rel));

    const float maxBounceAngle = CC_DEGREES_TO_RADIANS(65.0f); // i would have made a macro if i didnt know about this omg
    float angle = rel * maxBounceAngle;

    bool isTop = (paddle == m_paddleTop);
    float signY = isTop ? -1.0f : 1.0f;

    float vx = m_ballSpeed * sinf(angle);
    float vy = m_ballSpeed * signY * cosf(angle);

    vx += CCRANDOM_MINUS1_1() * 5.0f;

    const float minComponent = 40.0f;
    if (fabsf(vx) < minComponent) vx = (vx >= 0 ? minComponent : -minComponent);
    if (fabsf(vy) < minComponent) vy = (vy >= 0 ? minComponent : -minComponent);

    m_ballVel.x = vx;
    m_ballVel.y = vy;
    float mag = sqrtf(vx*vx + vy*vy);
    m_ballVel.x = (m_ballVel.x / mag) * m_ballSpeed;
    m_ballVel.y = (m_ballVel.y / mag) * m_ballSpeed;

    float yOffset = m_ballRadius + m_paddleHeight * 0.5f + 1.0f;
    if (isTop)
        m_ball->setPositionY(m_paddleTop->getPositionY() - yOffset);
    else
        m_ball->setPositionY(m_paddleBottom->getPositionY() + yOffset);

    m_ballSpeed = std::min(m_maxSpeedClamp, m_ballSpeed * 1.01f);
    normalizeBallVelocityToSpeed();
}

void PongGame::keyDown(cocos2d::enumKeyCodes key) {
    if (key == enumKeyCodes::KEY_Left || key == enumKeyCodes::CONTROLLER_Left || key == enumKeyCodes::KEY_ArrowLeft || key == enumKeyCodes::KEY_A) {
        if (!m_started) {
            scheduleUpdate();
            m_started = true;
        }
        m_holdLeft = true;
        m_holdRight = false;
        return;
    }
    if (key == enumKeyCodes::KEY_Right || key == enumKeyCodes::CONTROLLER_Right || key == enumKeyCodes::KEY_ArrowRight || key == enumKeyCodes::KEY_D) {
        if (!m_started) {
            scheduleUpdate();
            m_started = true;
        }
        m_holdRight = true;
        m_holdLeft = false;
        return;
    }
    Game::keyDown(key);
}
void PongGame::keyUp(cocos2d::enumKeyCodes key) {
    if (key == enumKeyCodes::KEY_Left || key == enumKeyCodes::CONTROLLER_Left || key == enumKeyCodes::KEY_ArrowLeft || key == enumKeyCodes::KEY_A) {
        m_holdLeft = false;
        return;
    }
    if (key == enumKeyCodes::KEY_Right || key == enumKeyCodes::CONTROLLER_Right || key == enumKeyCodes::KEY_ArrowRight || key == enumKeyCodes::KEY_D) {
        m_holdRight = false;
        return;
    }
    Game::keyUp(key);
}

void PongGame::normalizeBallVelocityToSpeed() {
    float vx = m_ballVel.x;
    float vy = m_ballVel.y;
    float mag = sqrtf(vx*vx + vy*vy);
    if (mag < 1e-6f) { // fkjaweoifjweioufhewaiufhbewiuf random nubmer idk
        float ang = CC_DEGREES_TO_RADIANS(30.0f);
        m_ballVel.x = m_ballSpeed * sinf(ang);
        m_ballVel.y = m_ballSpeed * cosf(ang);
        return;
    }
    m_ballVel.x = (vx / mag) * m_ballSpeed;
    m_ballVel.y = (vy / mag) * m_ballSpeed;
}

void PongGame::update(float dt) {
    CCLayer::update(dt);
    float move = 0.f;
    if (m_holdLeft) move -= 1.f;
    if (m_holdRight) move += 1.f;
    if (move != 0.f) {
        float px = m_paddleBottom->getPositionX();
        float speed = m_speed * dt;
        px += move * speed;
        float halfW = m_paddleWidth * 0.5f;
        px = std::min(px, m_playSize.width - halfW);
        px = std::max(px, halfW);
        m_paddleBottom->setPositionX(px);
    }
    float aiMove = 0.f;
    float topX = m_paddleTop->getPositionX();
    float bx = m_ball->getPositionX();
    float dx = bx - topX;
    if (fabsf(dx) > 2.0f) {
        float sign = (dx > 0) ? 1.f : -1.f;
        float aiStep = ((m_playerScore > 25) ? m_aiMaxSpeed + 55.f : m_aiMaxSpeed) * dt;
        float step = std::min(aiStep, fabsf(dx));
        topX += sign * step;
        float halfW = m_paddleWidth * 0.5f;
        topX = std::min(topX, m_playSize.width - halfW);
        topX = std::max(topX, halfW);
        m_paddleTop->setPositionX(topX);
    }

    CCPoint pos = m_ball->getPosition();
    CCPoint vel = m_ballVel;
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
    m_ball->setPosition(pos);

    float leftBound = 0;
    float rightBound = m_playSize.width;
    float ballRadius = m_ballRadius;
    // im so dumb, i wondered why it kept clipping randomly and it was because I FORGOT THE LAST F IN FABSF
    if (pos.x - ballRadius <= leftBound) {
        pos.x = leftBound + ballRadius;
        m_ballVel.x = fabsf(m_ballVel.x);
        m_ball->setPositionX(pos.x);
    } else if (pos.x + ballRadius >= rightBound) {
        pos.x = rightBound - ballRadius;
        m_ballVel.x = -fabsf(m_ballVel.x);
        m_ball->setPositionX(pos.x);
    }

    float topPaddleY = m_paddleTop->getPositionY();
    float bottomPaddleY = m_paddleBottom->getPositionY();
    float paddleHalfH = m_paddleHeight * 0.5f;
    if (m_ballVel.y > 0) {
        if (pos.y + ballRadius >= topPaddleY - paddleHalfH) {
            if (pos.x >= m_paddleTop->getPositionX() - m_paddleWidth*0.5f && pos.x <= m_paddleTop->getPositionX() + m_paddleWidth*0.5f) {
                reflectOffPaddle(m_paddleTop);
                pos.y = topPaddleY - paddleHalfH - ballRadius;
            }
        }
    } else {
        if (pos.y - ballRadius <= bottomPaddleY + paddleHalfH) {
            if (pos.x >= m_paddleBottom->getPositionX() - m_paddleWidth*0.5f &&
                pos.x <= m_paddleBottom->getPositionX() + m_paddleWidth*0.5f) {
                reflectOffPaddle(m_paddleBottom);
                pos.y = bottomPaddleY + paddleHalfH + ballRadius;
            }
        }
    }
    //log::debug("velocity = {}, speed = {}", m_ballVel, m_ballSpeed);
    m_ball->setPosition(pos);

    float topBound = m_playSize.height;
    float bottomBound = 0;
    if (pos.y - ballRadius > topBound) {
        m_playerScore += 1;
        onScore(true);
    } else if (pos.y + ballRadius < bottomBound) {
        m_opponentScore += 1;
        onScore(false);
    }

    m_timeSinceLastScore += dt;
    if (m_timeSinceLastScore >= m_noScoreThreshold) {
        increaseBallSpeed();
        m_timeSinceLastScore = 0.0f;
    }

    int totalScore = m_playerScore + m_opponentScore;
    if (totalScore >= 6) {
        float target = m_baseBallSpeed + (totalScore - 5) * 10.0f;
        m_ballSpeed = std::min(m_maxSpeedClamp, target);
        normalizeBallVelocityToSpeed();
    }
}

void PongGame::onScore(bool playerScored) {
    FMODAudioEngine::sharedEngine()->playEffect("chest07.ogg", 1, 0, GameManager::get()->m_sfxVolume);
    if (rand() % 2 == 1) { //playerScored
        m_ballSpeed = std::min(m_maxSpeedClamp, m_ballSpeed * 1.05f);
        resetBall(false);
    } else {
        m_ballSpeed = std::min(m_maxSpeedClamp, m_ballSpeed * 1.03f);
        resetBall(true);
    }
    float cx = m_playSize.width * 0.5f;
    m_paddleTop->setPositionX(cx);
    m_paddleBottom->setPositionX(cx);
    normalizeBallVelocityToSpeed();
    if (m_opponentScoreLbl && m_playerScoreLbl) {
        m_playerScoreLbl->setString(std::to_string(m_playerScore).c_str());
        m_opponentScoreLbl->setString(std::to_string(m_opponentScore).c_str());
    }
    if (playerScored) {
        Mod::get()->setSavedValue("points", Mod::get()->getSavedValue<int>("points", 0) + 10);
        if (auto menu = static_cast<SoggifyMenuLayer*>(this->getParent()->getChildByID("SoggifyMenuLayer"_spr))) {
            menu->updatePoints();
        }
        auto quests = Mod::get()->getSavedValue<std::vector<int>>("quests");
        auto allQuests = sogutils::getQuests();
        for (auto questID : quests) {
            if (questID < 0 || questID > allQuests.size() - 1) continue; // ok when would this actually happen
            auto quest = allQuests.at(questID);
            auto value = Mod::get()->getSavedValue<int>(fmt::format("q_{}", questID), 0);
            bool increaseValue = false;
            if (quest.type == QuestType::MinigameP) {
                log::debug("Increasing quest for {} (Pong) by +{} (before {})", questID, 1, value);
                Mod::get()->setSavedValue(fmt::format("q_{}", questID), std::clamp(value + 1, 0, (int)quest.maxProgress));
            }
        }
    }
}
void PongGame::increaseBallSpeed() {
    m_ballSpeed = std::clamp(m_ballSpeed, m_ballSpeed * m_speedRampFactor, m_maxSpeedClamp);
    normalizeBallVelocityToSpeed();
}
