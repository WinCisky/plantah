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
        int ID = -1;
        short HEIGHT = 1;
        std::vector<short> RESOURCES = { 2, 2, 2};
    };

    struct PlayerChoice
    {
        short m_playerId;
        short m_choice;
    };

    const short CHOICES = 7;
    const short MAX_RESOURCES = 10;
    const short RESOURCES_TRESHOLD = 7;
    const short RESOURCE_INCREMENT = 5;
    const short RESOURCE_DECREMENT = 3;
    const short TRINITY_INCREMENT = 3;
    const short TRINITY_DECREMENT = 1;
    const short GROWTH_DECREMENT = 1;
    const short WEATHER_INCREMENT = 1;
    const short WINNING_HEIGHT = 5;
    const std::vector<std::string> WEATHERS = {"sunny", "rainy", "moon"};

    int m_lobby = -1;
    std::string m_weather = "";
    std::vector<short> m_choices = {-1, -1, -1};
    std::vector<Player> m_players;
    
    int m_buttonSelected, m_buttonConfirmed;
    std::map<std::string, sf::RectangleShape> m_buttons;
    sf::Text m_playText;

    sf::Vector2u m_screenSize, m_screenQuarter, m_quarterSixth;
    bool m_isLandscape; 

    void placeButtons();
    void sendChoice(short index);
    void updatePlayers(std::vector<PlayerChoice> & choices);
    void incrementPlayerResource(int playerId, short resource, short increment);
    void decrementPlayerResource(int playerId, short resource, short decrement);
    
public:
    Scene_Play(GameEngine * gameEngine, short lobbyId, std::vector<int> players);
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