#pragma once

#include "Scene.h"
#include <map>
#include <memory>

#include "EntityManager.h"
#include "SimpleJsonParser.h"

class Scene_Play : public Scene
{
    struct Player
    {
        short HEIGHT = 1;
        std::vector<short> RESOURCES = { 3, 3, 3};
    };

    int m_lobby = -1;
    short m_weather = -1;
    std::vector<short> m_choices = { -1, -1, -1};
    std::vector<Player> m_players;
    
    int m_buttonSelected, m_buttonConfirmed;
    std::map<std::string, sf::RectangleShape> m_buttons;
    sf::Text m_playText;

    sf::Vector2u m_screenSize, m_screenQuarter, m_quarterSixth;
    bool m_isLandscape; 

    void placeButtons();
    void sendChoice(short index);
    
public:
    Scene_Play(GameEngine * gameEngine);
    void sRender();
    void sReceive(std::string & message);

protected:

    SimpleJsonParser m_jsonParser;
    
    void init();
    void update();
    void onEnd();
    void sDoAction(const Action & action);
    void sDoActionMouse(const Action & action, const Vec2 & pos);
    void sSend(std::string & message);
};