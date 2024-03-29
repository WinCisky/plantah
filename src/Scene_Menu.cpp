#include "Scene_Menu.h"
#include "Scene_Lobby.h"
#include "Scene_GameOver.h"
#include "GameEngine.h"
#include "Action.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Scene_Menu::Scene_Menu(GameEngine * gameEngine) 
    : Scene(gameEngine)
{
    init();
}

void Scene_Menu::init() 
{
    m_title = "Scene menu";

    registerAction(sf::Keyboard::Key::W, "UP");
    registerAction(sf::Keyboard::Key::S, "DOWN");
    registerAction(sf::Keyboard::Key::Enter, "SELECT");
    registerAction(sf::Keyboard::Key::Escape, "QUIT");
    registerActionMouse(sf::Mouse::Button::Left, "CLICK");

    m_menuStrings.push_back("Play");
    m_menuStrings.push_back("Test");

    m_menuText.setFont(m_game->assets().getFont("assets/tech.ttf"));
    m_menuText.setCharacterSize(20);
}

void Scene_Menu::update() 
{
    m_currentFrame++;
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
        else if (action.name() == "SELECT")
        {
            if (m_menuStrings.at(m_selectedMenuIndex) == "Play")
            {
                m_game->changeScene("LOBBY", std::make_shared<Scene_Lobby>(m_game), true);
            }
            else if (m_menuStrings.at(m_selectedMenuIndex) == "Test")
            {
                m_game->changeScene("GAMEOVER", std::make_shared<Scene_GameOver>(m_game, 0), true);
            }
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
        // std::cout << pos.x << " " << pos.y << std::endl;
        // check what buton was clicked
        auto firstButtonPos = sf::Vector2f(100, 200);
        auto buttonSize = sf::Vector2f(100, 50);
        for (size_t i = 0; i < m_menuStrings.size(); i++)
        {
            if (pos.x > firstButtonPos.x && pos.x < firstButtonPos.x + buttonSize.x &&
                pos.y > firstButtonPos.y + 100 * i && pos.y < firstButtonPos.y + 100 * i + buttonSize.y)
            {
                // std::cout << "Button " << i << " clicked" << std::endl;
                if (m_menuStrings.at(i) == "Play")
                {
                    m_game->changeScene("LOBBY", std::make_shared<Scene_Lobby>(m_game), true);
                }
                else if (m_menuStrings.at(i) == "Test")
                {
                    m_game->changeScene("GAMEOVER", std::make_shared<Scene_GameOver>(m_game, 0), true);
                }
            }
        }
    }
}

void Scene_Menu::sRender() 
{
    m_menuText.setString(m_title);
    m_menuText.setOrigin(m_menuText.getGlobalBounds().width / 2.f + m_menuText.getLocalBounds().left,
                        m_menuText.getGlobalBounds().height / 2.f + m_menuText.getLocalBounds().top);
    m_menuText.setPosition(m_game->window().getSize().x / 2.f, 100);
    m_game->window().draw(m_menuText);

    // center
    auto windowSize = m_game->window().getSize();
    float firstButttonHeight = (m_game->window().getSize().y / 2.f) - 25 - (50 * (m_menuStrings.size() - 1)); 

    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        // draw btn rectangle
        sf::RectangleShape rectangle(sf::Vector2f(100, 50));

        rectangle.setFillColor(sf::Color(50, 50, 50));
        rectangle.setOutlineThickness(0);
        rectangle.setOutlineColor(sf::Color(255, 255, 255));
        if (m_selectedMenuIndex == i)
        {
            rectangle.setOutlineThickness(1);
        }
        rectangle.setPosition(sf::Vector2f(windowSize.x / 2.f - 50, firstButttonHeight + 100 * i));

        // set text at center
        m_menuText.setString(m_menuStrings.at(i));
        m_menuText.setFillColor(sf::Color(255, 255, 255));
        m_menuText.setOrigin(m_menuText.getGlobalBounds().width / 2.f + m_menuText.getLocalBounds().left,
                            m_menuText.getGlobalBounds().height / 2.f + m_menuText.getLocalBounds().top);
        m_menuText.setPosition(rectangle.getPosition() + (rectangle.getSize() / 2.f));

        m_game->window().draw(rectangle);
        m_game->window().draw(m_menuText);
    }
    
    
}

void Scene_Menu::sReceive(std::string & message) 
{
    // parse
    m_parser.parse(message);
    std::string type = m_parser.get("type");
    if (type == "id")
    {
        // std::cout << "Received id: " << m_parser.getNumber("id") << std::endl;
        int id = std::stoi(m_parser.getNumber("id"));
        m_game->playerId = id;
    }
    
}