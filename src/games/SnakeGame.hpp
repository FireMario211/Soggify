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

        bool m_growing;
        float m_currentSpeed = 0.18f;
        float m_squareCWidth;

        bool setup() override;
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
