#include "Scene_Menu.h"
#include "GameEngine.h"
#include "Action.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Scene_Menu::Scene_Menu(GameEngine * gameEngine) 
    : Scene(gameEngine)
    , m_menuText(sf::Text(m_game->assets().getFont("assets/tech.ttf"), "", 20))
{
    init();
}

void Scene_Menu::init() 
{
    m_title = "Scene menu";

    registerAction(sf::Keyboard::Key::W, "UP");
    registerAction(sf::Keyboard::Key::S, "DOWN");
    registerAction(sf::Keyboard::Key::Escape, "QUIT");
    registerActionMouse(sf::Mouse::Button::Left, "CLICK");

    m_menuStrings.push_back("one");
    m_menuStrings.push_back("two");
    m_menuStrings.push_back("three");

    m_menuText.setFont(m_game->assets().getFont("assets/tech.ttf"));
    m_menuText.setCharacterSize(20);
}

void Scene_Menu::update() 
{
    sRender();
}

void Scene_Menu::onEnd() 
{
    m_hasEnded = true;
    m_game->quit();
}

void Scene_Menu::sDoAction(const Action & action) 
{
    if (action.type() == "START")
    {
        if (action.name() == "UP")
        {
            m_selectedMenuIndex = m_selectedMenuIndex == 0 ? (m_menuStrings.size() - 1) : (m_selectedMenuIndex - 1);
        }
        else if (action.name() == "DOWN")
        {
            m_selectedMenuIndex = m_menuStrings.size() == 0 ? 0 : ((m_selectedMenuIndex + 1) % m_menuStrings.size());
        }
        else if (action.name() == "QUIT")
        {
            onEnd();
        }
    }
}

void Scene_Menu::sDoActionMouse(const Action & action, const Vec2 & pos)
{
    if (action.type() == "START")
    {
        std::cout << pos.x << " " << pos.y << std::endl;
    }
}

void Scene_Menu::sRender() 
{
    
    m_menuText.setString(m_title);
    m_menuText.setFillColor(sf::Color(255, 255, 255));
    m_menuText.setPosition(sf::Vector2f(100, 100));
    m_game->window().draw(m_menuText);

    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        sf::RectangleShape rectangle(sf::Vector2f(100, 50));
        rectangle.setPosition(sf::Vector2f(100, 200 + 100 * i));
        rectangle.setFillColor(sf::Color(150, 150, 150));
        rectangle.setOutlineThickness(0);
        if (m_selectedMenuIndex == i)
        {
            rectangle.setOutlineColor(sf::Color(255, 255, 255));
            rectangle.setOutlineThickness(1);
        }
        m_game->window().draw(rectangle);

        m_menuText.setString(m_menuStrings.at(i));
        m_menuText.setFillColor(sf::Color(255, 255, 255));
        m_menuText.setPosition(sf::Vector2f(100, 200 + 100 * i));
        m_game->window().draw(m_menuText);
    }
    
    
}