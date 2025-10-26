#include "MGPopup.hpp"
#include "Game.hpp"
#include "PongGame.hpp"
#include "SnakeGame.hpp"
#include <layers/SoggifyMenuLayer.hpp>
#include <defs/utils.hpp>

//std::vector<std::string> s_games = {"Pong", "Snake", "Flappy Sog"};
std::vector<std::string> s_games = {"Pong", "Snake"}; // was going to add "Spin Wheel" but i ran out of time, so maybe if i have enough time idk

bool MGPopup::setup() {
    this->setTitle("minigames!");
    //Build<CCMenu>::create().parent(m_mainLayer).store(m_gameMenu);
    
    for (int i = 0; i < s_games.size(); i++) {
        Build<ButtonSprite>::create(s_games.at(i).c_str(), "bigFont.fnt", "GJ_button_01.png")
            .scale(0.8f)
            .intoMenuItem([i]() {
                Game* game = nullptr;
                switch (i) {
                    case 0: // Pong
                        game = PongGame::create();
                        break;
                    case 1: // Snake
                        game = SnakeGame::create();
                        break;
                    case 2: // Flappy Sog
                        break;
                }
                if (game == nullptr) return FLAlertLayer::create("Error", "Couldn't get <cy>mini game</c>, please report this to the developer of this mod", "OK")->show();
                game->show();
            })
            .parentAtPos(m_buttonMenu, Anchor::Center, {0, (i * -35.f) + 20.f});
    }
    return true;
}

void MGPopup::onClose(CCObject* sender) {
    if (auto menu = static_cast<SoggifyMenuLayer*>(this->getParent()->getChildByID("SoggifyMenuLayer"_spr))) {
        menu->updatePoints();
    }
    Popup::onClose(sender);
}
