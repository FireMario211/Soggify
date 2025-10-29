#pragma once
#include "Game.hpp"

class WheelNode : public cocos2d::CCNode {
public:
    int m_sectors;
    std::vector<cocos2d::ccColor4F> m_colors;

    static WheelNode* create(int sectors, const std::vector<cocos2d::ccColor4F>& colors) {
        auto w = new WheelNode();
        if (w && w->initWithSectors(sectors, colors)) {
            w->autorelease();
            return w;
        }
        CC_SAFE_DELETE(w);
        return nullptr;
    }

    bool initWithSectors(int s, const std::vector<cocos2d::ccColor4F>& c) {
        m_sectors = s;
        m_colors = c;
        return true;
    }

    void setColors(const std::vector<cocos2d::ccColor4F>& colors) {
        m_colors = colors;
    }

    virtual void draw() {
        CCNode::draw();
        float step = 360.0f / m_sectors;
        cocos2d::CCPoint center = cocos2d::CCPointZero;
        float radius = 100.0f;
        int smoothFactor = 32;

        for (int i = 0; i < m_sectors; ++i) {
            float startAngle = CC_DEGREES_TO_RADIANS(i * step);
            float endAngle = CC_DEGREES_TO_RADIANS((i + 1) * step);
            cocos2d::ccColor4F color = m_colors[i % m_colors.size()];

            std::vector<cocos2d::CCPoint> verts;
            verts.push_back(center);
            for (int j = 0; j <= smoothFactor; ++j) {
                float t = (float)j / smoothFactor;
                float ang = startAngle + (endAngle - startAngle) * t;
                verts.push_back(ccp(radius * cosf(ang), radius * sinf(ang)));
            }

            ccDrawSolidPoly(&verts[0], (int)verts.size(), color);
        }
        cocos2d::ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(5.0f);
        cocos2d::ccDrawColor4F(0.0f, 0.0f, 0.0f, 1.0f);
        cocos2d::ccDrawCircle(center, radius, 0, 100, false);
    }
};


class SpinWheelGame : public Game {
    protected:
        WheelNode* m_wheel;
        cocos2d::CCNode* m_container;
        std::vector<int> m_rewards;
        std::vector<cocos2d::ccColor4F> m_wheelColors;
        cocos2d::CCParticleSystemQuad* m_emitter = nullptr;
        bool m_isSpinning = false;
        float m_lastAngle = 0.0f;
        int m_numSectors = 8;

        void LETSGOGAMBLING();
        void onSpinComplete();
        void update(float) override;
        void setRewards();
        bool setup() override;
        void onClose(cocos2d::CCObject* pSender) override {
            if (m_isSpinning) return;
            Game::onClose(pSender);
        };
    public:
        static SpinWheelGame* create() {
            auto ret = new SpinWheelGame();
            if (ret->initGame("Spin the Wheel!!!")) {
                return ret;
            }
            delete ret;
            return nullptr;
        }
};
