#pragma once
#include "Game.hpp"

class SnakeGame : public Game {
    protected:
        enum Direction { UP, DOWN, LEFT, RIGHT };
        cocos2d::extension::CCScale9Sprite* m_bg;
        std::deque<cocos2d::CCPoint> m_snakeCells;
        std::deque<cocos2d::CCSprite*> m_snakeSprites;
        cocos2d::CCPoint m_foodCell;
        cocos2d::CCSprite* m_foodSprite;
        
        Direction m_dir = UP;
        std::deque<Direction> m_inputBuffer;
        const size_t INPUT_BUFFER_MAX = 3;
        cocos2d::CCPoint m_touchStart;

        bool m_growing;
        float m_currentSpeed = 0.18f;
        float m_squareCWidth;

        bool setup() override;
#ifndef GEODE_IS_DESKTOP
        inline cocos2d::CCPoint getFirstTouchPos(cocos2d::CCTouch* t) {
            return t ? t->getLocation() : ccp(0,0);
        }
        virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override {
            if (!Game::ccTouchBegan(pTouch, pEvent)) return false;
            m_touchStart = getFirstTouchPos(pTouch);
            return true;
        }
        virtual void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) override {
            Game::ccTouchEnded(touch, event);
            cocos2d::CCPoint d = ccpSub(getFirstTouchPos(touch), m_touchStart);
            if (fabsf(d.x) < 10.f && fabsf(d.y) < 10.f) return;
            if (fabsf(d.x) > fabsf(d.y)) {
                if (d.x > 0) tryChangeDirection(RIGHT); else tryChangeDirection(LEFT);
            } else {
                if (d.y > 0) tryChangeDirection(UP); else tryChangeDirection(DOWN);
            }
        }
#endif
        void keyDown(cocos2d::enumKeyCodes key) override;
        cocos2d::CCPoint getPos(uint8_t x, uint8_t y);
        cocos2d::CCPoint getPos(cocos2d::CCPoint pos) {
            return getPos(pos.x, pos.y);
        }
        float gridScale();
        float gridScale(float width);
        void tryChangeDirection(Direction newDir) {
            Direction last = m_inputBuffer.empty() ? m_dir : m_inputBuffer.back();
            if (isOpposite(last, newDir)) return;
            
            if (!m_inputBuffer.empty() && m_inputBuffer.back() == newDir) return;
            if (m_inputBuffer.size() < INPUT_BUFFER_MAX) {
                m_inputBuffer.push_back(newDir);
            }
        }
        bool isOpposite(Direction a, Direction b) {
            // disallow reversing
            return (a == LEFT  && b == RIGHT) ||
                   (a == RIGHT && b == LEFT) ||
                   (a == UP    && b == DOWN) ||
                   (a == DOWN  && b == UP);
        }
        void updateColors();
        void tick(float);
        void spawnFood();
        void restart();
        void gameOver(bool);

        void updateSpeed(float newInterval) {
            unschedule(schedule_selector(SnakeGame::tick));
            schedule(schedule_selector(SnakeGame::tick), newInterval);
        }
    public:
        static SnakeGame* create() {
            auto ret = new SnakeGame();
            if (ret->initGame("Snake")) {
                return ret;
            }
            delete ret;
            return nullptr;
        }
};
