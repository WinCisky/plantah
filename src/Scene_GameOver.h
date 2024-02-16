#pragma once

#include "Scene.h"
#include <map>
#include <memory>
#include <deque>

#include "EntityManager.h"
#include "SimpleJsonParser.h"

class Scene_GameOver : public Scene
{
    const std::string VICTORY = "You won!";
    const std::string DEFEAT = "You lost!";
    const std::string CONTINUE = "Continue";

    bool m_isWinner;
protected:
    std::string m_title;
    sf::Text m_menuText;
    sf::RectangleShape m_buttonContinue;

    void init();
    void update();
    void onEnd();
    void sDoAction(const Action & action);
    void sDoActionMouse(const Action & action, const Vec2 & pos);
    void sSend(std::string & message) {};

public:

    Scene_GameOver(GameEngine * gameEngine = nullptr, int winner = -1);
    void sRender();
    void sReceive(std::string & message) {};
};