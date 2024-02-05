#include "Scene_Lobby.h"
#include "GameEngine.h"
#include "Action.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Scene_Play.h"

Scene_Lobby::Scene_Lobby(GameEngine * gameEngine) 
    : Scene(gameEngine)
    , m_menuText(sf::Text(m_game->assets().getFont("assets/tech.ttf"), "", 20))
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
}

void Scene_Lobby::update() 
{
    sRender();
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
    
    
}