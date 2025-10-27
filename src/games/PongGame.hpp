#pragma once
#include "Game.hpp"

class PongGame : public Game {
    protected:
        cocos2d::extension::CCScale9Sprite* m_bg;
        cocos2d::CCSize m_playSize;

        bool m_started = false;
        //float m_paddleWidth = 60.0f;
        float m_paddleWidth = 45.0f;
        float m_paddleHeight = 12.0f;
        float m_ballRadius = 6.0f;

        float m_speed = 220.0f;
        bool m_holdLeft = false;
        bool m_holdRight = false;
        cocos2d::CCSprite* m_paddleTop;
        SimplePlayer* m_paddleBottom;
        cocos2d::CCSprite* m_ball;
        cocos2d::CCLabelBMFont* m_playerScoreLbl;
        cocos2d::CCLabelBMFont* m_opponentScoreLbl;

        cocos2d::CCPoint m_ballVel;
        float m_ballSpeed = 170.f;
        float m_baseBallSpeed = 170.f;
        float m_maxSpeedClamp = 490.f;

        float m_aiMaxSpeed = 120.f;

        unsigned int m_playerScore;
        unsigned int m_opponentScore;

        float m_timeSinceLastScore;
        float m_noScoreThreshold = 10.f;
        float m_speedRampFactor;


        void onScore(bool playerScored);
        void increaseBallSpeed();
        void reflectOffPaddle(cocos2d::CCNode* paddle);
        void resetBall(bool serveToPlayer);
        void normalizeBallVelocityToSpeed();
        bool setup() override;
        void keyDown(cocos2d::enumKeyCodes key) override;
        void keyUp(cocos2d::enumKeyCodes key) override;

        void restart();
        void gameOver(bool);
        void update(float dt) override;
    public:
        static PongGame* create() {
            auto ret = new PongGame();
            if (ret->initGame("Pong")) {
                return ret;
            }
            delete ret;
            return nullptr;
        }
};
