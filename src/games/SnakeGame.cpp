// partly taken inspiration from googles own snake game, also THE INPUT BUFFERS ARE AMAZING so i added those too
#include "SnakeGame.hpp"
#include <defs/geode.hpp>
#include <layers/SoggifyMenuLayer.hpp>

CCPoint SnakeGame::getPos(uint8_t x, uint8_t y) {
    float M = 3.f;
    float tileSize = (205.f - 2.f*M) / 20.f;
    float px = M + x * (tileSize);
    float py = M + y * (tileSize);
    return {px, py};
}
float SnakeGame::gridScale(float width) {
    float M = 3.f;
    float tileSize = (205.f - 2.f*M) / 20.f;
    float scale = tileSize / width;
    return scale;
}
float SnakeGame::gridScale() {
    return gridScale(m_squareCWidth);
}

bool SnakeGame::setup() {
    Loader::get()->queueInMainThread([](){
        FLAlertLayer::create("How to", "Move around by holding <cg>any of the arrow keys</c>!\nEach time <cy>Sog</c> eat an apple sog, you gain <cy>+1 Sog Point</c>!", "OK")->show();
    });
    uint8_t s_grid = 20;
    CCSize s_cellSize = {205.f, 205.f};
    Build<CCScale9Sprite>::create("square02_small.png")
        .opacity(100)
        .contentSize(s_cellSize)
        .parentAtPos(m_mainLayer, Anchor::Center, {0, -10})
        .store(m_bg);
    m_squareCWidth = CCSprite::create("square.png")->getContentWidth();

    // === Grid === //
    for (int y = 0; y < s_grid; y++) {
        for (int x = 0; x < s_grid; x++) {
            auto tile = CCSprite::create("square.png");
            tile->setScale(gridScale());
            tile->setAnchorPoint({0,0});
            // 180, 230, 100
            int checker = ((x + y) & 1) ? 30 : 0;
            tile->setColor(ccc3(150 + checker, 200 + checker, 70 + checker));
            m_bg->addChildAtPosition(tile, Anchor::BottomLeft, getPos(x,y));
        }
    }
    // === Grid === //
    restart();
    return true;
}

void SnakeGame::restart() {
    this->setTitle("Snake (0 points)");
    m_dir = Direction::UP;
    m_snakeCells.clear();
    for (auto s : m_snakeSprites) { s->removeFromParentAndCleanup(true); }
    m_snakeSprites.clear();
    for (int i=0; i<4; i++) {
        CCPoint cell = {10.f - i, 10.f};
        m_snakeCells.push_back(cell);
        CCSprite* seg = CCSprite::create("SOGGY.png"_spr);
        seg->setAnchorPoint({0, 0});
        seg->setScaleX(gridScale(seg->getContentWidth()));
        seg->setScaleY(gridScale(seg->getContentHeight()));
        m_bg->addChildAtPosition(seg, Anchor::BottomLeft, getPos(cell));
        m_snakeSprites.push_back(seg);
    }
    spawnFood();
    updateSpeed(0.18f);
}

void SnakeGame::updateColors() {
    size_t len = m_snakeSprites.size();
    for (size_t i=0;i<len;++i) {
        float t = (len > 1) ? (float)i / (float)(len - 1) : 0.0f;
        int bright = (int) (255 - t * 165.0f); // 255 -> 90
        m_snakeSprites[i]->setColor(ccc3(bright, bright, bright));
    }
}

void SnakeGame::keyDown(cocos2d::enumKeyCodes key) {
    if (key == enumKeyCodes::KEY_Left || key == enumKeyCodes::CONTROLLER_Left || key == enumKeyCodes::KEY_ArrowLeft || key == enumKeyCodes::KEY_A)
        return tryChangeDirection(Direction::LEFT);
    if (key == enumKeyCodes::KEY_Right || key == enumKeyCodes::CONTROLLER_Right || key == enumKeyCodes::KEY_ArrowRight || key == enumKeyCodes::KEY_D)
        return tryChangeDirection(Direction::RIGHT);
    if (key == enumKeyCodes::KEY_Up || key == enumKeyCodes::CONTROLLER_Up || key == enumKeyCodes::KEY_ArrowUp || key == enumKeyCodes::KEY_W)
        return tryChangeDirection(Direction::UP);
    if (key == enumKeyCodes::KEY_Down || key == enumKeyCodes::CONTROLLER_Down || key == enumKeyCodes::KEY_ArrowDown || key == enumKeyCodes::KEY_D)
        return tryChangeDirection(Direction::DOWN);
    Game::keyDown(key);
}

void SnakeGame::tick(float dt) {
    if (!m_inputBuffer.empty()) {
        m_dir = m_inputBuffer.front();
        m_inputBuffer.pop_front();
    }
    uint8_t gridSize = 20;
    CCPoint head = m_snakeCells.front();
    CCPoint nh = head;
    switch (m_dir) {
        case Direction::UP:    nh.y += 1; break;
        case Direction::DOWN:  nh.y -= 1; break;
        case Direction::LEFT:  nh.x -= 1; break;
        case Direction::RIGHT: nh.x += 1; break;
    }
    if (nh.x < 0) nh.x = gridSize - 1;
    else if (nh.x >= gridSize) nh.x = 0;
    if (nh.y < 0) nh.y = gridSize - 1;
    else if (nh.y >= gridSize) nh.y = 0;

    bool collideSelf = false;
    for (int i = 0; i < m_snakeCells.size(); i++) {
        if (m_snakeCells[i].equals(nh)) {
            if (i == m_snakeCells.size() - 1 && !m_growing) {
                // allowed
            } else {
                collideSelf = true;
                break;
            }
        }
    }
    if (collideSelf) {
        gameOver(false);
        return;
    }
    m_snakeCells.push_front(nh);
    CCSprite* newHeadSprite = CCSprite::create("SOGGY.png"_spr);
    newHeadSprite->setAnchorPoint({0, 0});
    newHeadSprite->setScaleX(gridScale(newHeadSprite->getContentWidth()));
    newHeadSprite->setScaleY(gridScale(newHeadSprite->getContentHeight()));
    m_bg->addChildAtPosition(newHeadSprite, Anchor::BottomLeft, getPos(nh));
    m_snakeSprites.push_front(newHeadSprite);

    if (nh.equals(m_foodCell)) {
        m_growing = true;
        if (m_foodSprite) {
            m_foodSprite->removeFromParentAndCleanup(true);
            m_foodSprite = nullptr;
        }
        spawnFood();
    }

    if (!m_growing) {
        CCSprite* tail = m_snakeSprites.back();
        tail->removeFromParentAndCleanup(true);
        m_snakeSprites.pop_back();
        m_snakeCells.pop_back();
    } else {
        m_growing = false;
    }

    auto itCell = m_snakeCells.begin();
    auto itSpr = m_snakeSprites.begin();

    // dont question it
    for (; itCell != m_snakeCells.end() && itSpr != m_snakeSprites.end(); ++itCell, ++itSpr) {
        (*itSpr)->setPosition(getPos(*itCell));
    }
    updateColors();
}

void SnakeGame::spawnFood() {
    int points = (m_snakeCells.size() - 4);
    if (points != 0) {
        FMODAudioEngine::sharedEngine()->playEffect("counter003.ogg", 1, 0, GameManager::get()->m_sfxVolume);
        Mod::get()->setSavedValue("points", Mod::get()->getSavedValue<int>("points", 0) + 1);
        if (auto menu = static_cast<SoggifyMenuLayer*>(this->getParent()->getChildByID("SoggifyMenuLayer"_spr))) {
            menu->updatePoints();
        }
        this->setTitle(fmt::format("Snake ({} point{})", points, (points == 1) ? "" : "s"));

        if (points % 10 == 0) {
            FMODAudioEngine::sharedEngine()->playEffect("chest07.ogg", 1, 0, GameManager::get()->m_sfxVolume);
            const float baseTick = 0.18f;
            const int maxScore = 200;

            const float minTick = 0.01f;
            float factor = (float)points / 200.f;
            if (factor > 1.0f) factor = 1.0f;

            float newTickInterval = baseTick - (baseTick - minTick) * factor;
            log::debug("update speed! ({})", newTickInterval);
            updateSpeed(newTickInterval);
        }
    }
    uint8_t gridSize = 20;
    if (m_foodSprite) {
        m_foodSprite->removeFromParentAndCleanup(true);
        m_foodSprite = NULL;
    }

    std::vector<CCPoint> freeCells;
    for (int y=0;y<gridSize;y++) {
        for (int x=0;x<gridSize;x++) {
            CCPoint c = CCPoint(x,y);
            bool occupied = false;
            for (auto &sc : m_snakeCells) {
                if (sc.equals(c)) { occupied = true; break; }
            }
            if (!occupied) freeCells.push_back(c);
        }
    }
    if (freeCells.empty()) {
        // win, though i actually dont know if this works...
        gameOver(true);
        return;
    }
    int idx = rand() % freeCells.size();
    CCPoint chosen = freeCells[idx];
    m_foodSprite = CCSprite::create("square.png");

    m_foodSprite->setAnchorPoint({0, 0});
    m_foodSprite->setScale(gridScale());
    m_foodSprite->setColor(ccc3(220, 40, 40));
    m_bg->addChildAtPosition(m_foodSprite, Anchor::BottomLeft, getPos(chosen));
    m_foodCell = chosen;
}

void SnakeGame::gameOver(bool win) {
    unschedule(schedule_selector(SnakeGame::tick));
    if (win) {
        FLAlertLayer::create("congrats!!!", "you won! For that, you get <cy>+500 extra sog points</c>!\nYou will need to reopen this menu to play again.", "thank you");
        Mod::get()->setSavedValue("points", Mod::get()->getSavedValue<int>("points", 0) + 500);
        if (auto menu = static_cast<SoggifyMenuLayer*>(this->getParent()->getChildByID("SoggifyMenuLayer"_spr))) {
            menu->updatePoints();
        }
    } else {
        FMODAudioEngine::sharedEngine()->playEffect("explode_11.ogg", 1, 0, GameManager::get()->m_sfxVolume);
        geode::createQuickPopup("you lost!", "unfortunately <cy>Sog</c> crashed into itself.\nWould you like to <cg>restart</c> and play again?", "nuh uh", "yes", [this](auto, auto btn2) {
            if (btn2) {
                restart();
            } else {
                this->onClose(nullptr);
            }
        }, true, false);
    }
}
