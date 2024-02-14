#include "Scene_Lobby.h"
#include "GameEngine.h"
#include "Action.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Scene_Play.h"

Scene_Lobby::Scene_Lobby(GameEngine * gameEngine) 
    : Scene(gameEngine)
    , m_menuText(sf::Text(m_game->assets().getFont("assets/tech.ttf"), "", 20))
    , m_lobbyText(sf::Text(m_game->assets().getFont("assets/tech.ttf"), "", 10))
{
    init();
}

void Scene_Lobby::init() 
{
    m_title = "Lobby menu";

    registerAction(sf::Keyboard::Key::W, "UP");
    registerAction(sf::Keyboard::Key::S, "DOWN");
    registerAction(sf::Keyboard::Key::Enter, "SELECT");
    registerAction(sf::Keyboard::Key::Escape, "QUIT");
    registerActionMouse(sf::Mouse::Button::Left, "CLICK");

    m_menuText.setFont(m_game->assets().getFont("assets/tech.ttf"));
    m_menuText.setCharacterSize(20);

    // send play message
    std::string message = "{\"type\":\"play\"}";
    sSend(message);
}

void Scene_Lobby::update() 
{
    m_currentFrame++;
}

void Scene_Lobby::onEnd() 
{
    m_hasEnded = true;
    m_game->quit();
}

void Scene_Lobby::sDoAction(const Action & action) 
{
    if (action.type() == "START")
    {
        if (action.name() == "QUIT")
        {
            onEnd();
        }
    }
}

void Scene_Lobby::sDoActionMouse(const Action & action, const Vec2 & pos)
{
    if (action.type() == "START")
    {
    }
}

void Scene_Lobby::sRender() 
{
    
    m_menuText.setString(m_title);
    m_menuText.setFillColor(sf::Color(255, 255, 255));
    m_menuText.setPosition(sf::Vector2f(100, 100));
    m_game->window().draw(m_menuText);

    std::string dots = "";
    if (m_currentFrame % 30 == 0)
    {
        m_dots = (m_dots + 1) % (MAX_DOTS + 1);
    }
    for (int i = 0; i < m_dots; i++)
    {
        dots += ".";
    }

    m_lobbyText.setString("Waiting for match" + dots);
    m_lobbyText.setFillColor(sf::Color(255, 255, 255));
    m_lobbyText.setPosition(sf::Vector2f(100, 200));
    m_game->window().draw(m_lobbyText);    
    
}

void Scene_Lobby::sSend(std::string & message) 
{
    m_game->sendNetworkMessage(message);
};

void Scene_Lobby::sReceive(std::string & message)
{
    // do stuff
    std::cout << "Received message in lobby: " << message << std::endl;
    // json parse message
    m_jsonParser.parse(message);
    std::string type = m_jsonParser.get("type");
    if (type == "matchFound")
    {
        std::string lobbyId = m_jsonParser.getNumber("lobby");
        std::cout << "Match found in lobby: " << lobbyId << std::endl;
        short lobby = static_cast<short>(std::stoi(lobbyId));
        std::string playersArrayString = m_jsonParser.getArray("players");
        std::string players = playersArrayString.substr(1, playersArrayString.size() - 2);
        std::vector<int> playersVector;
        std::string player = "";
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i] == ',')
            {
                playersVector.push_back(std::stoi(player));
                player = "";
            }
            else
            {
                player += players[i];
            }
        }
        playersVector.push_back(std::stoi(player));
        m_game->changeScene("GAME", std::make_shared<Scene_Play>(m_game, lobby, playersVector), true);
    }
};