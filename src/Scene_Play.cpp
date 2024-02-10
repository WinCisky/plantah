#include "Scene_Play.h"
#include "Scene_Menu.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"

#include <iostream>

Scene_Play::Scene_Play(GameEngine * gameEngine)
    : Scene(gameEngine)
    // , m_gridText(m_game->assets().getFont("assets/tech.ttf"), "", 12)
{
    init();
}

void Scene_Play::init()
{
    registerAction(sf::Keyboard::Key::Escape, "QUIT");
    registerActionMouse(sf::Mouse::Button::Left, "CLICK");

    // assign buttons size and position
    placeButtons();
    m_buttonSelected = -1;
}

void Scene_Play::placeButtons()
{
    auto screenSize = m_game->window().getSize();
    auto screenQuarters = screenSize / 4u;
    auto quarterSixth = screenQuarters / 6u;
    bool isLandscape = screenSize.x > screenSize.y;

    // place first bottom left
    auto firstButtonSize = isLandscape 
        ? sf::Vector2f(100, 100) // TODO 
        : sf::Vector2f(screenQuarters.x, screenQuarters.x);
    auto firstButtonPos = isLandscape 
        ? sf::Vector2f(100, 100) // TODO
        : sf::Vector2f(
            quarterSixth.x, 
            screenSize.y - quarterSixth.y - firstButtonSize.x
        );
        
    // place second bottom center
    auto secondButtonSize = firstButtonSize;
    auto secondButtonPos = isLandscape
        ? sf::Vector2f(100, 100) // TODO
        : sf::Vector2f(
            (screenSize.x / 2.0) - (secondButtonSize.x / 2.0), 
            screenSize.y - quarterSixth.y - secondButtonSize.x
        );
    // place third bottom right
    auto thirdButtonSize = firstButtonSize;
    auto thirdButtonPos = isLandscape
        ? sf::Vector2f(100, 100) // TODO
        : sf::Vector2f(
            screenSize.x - quarterSixth.x - thirdButtonSize.x, 
            screenSize.y - quarterSixth.y - thirdButtonSize.x
        );

    auto firstButton = sf::RectangleShape(firstButtonSize);
    firstButton.setPosition(firstButtonPos);
    firstButton.setFillColor(sf::Color::Red);
    firstButton.setOutlineColor(sf::Color::White);
    m_buttons.insert(std::make_pair("first", firstButton));
    auto secondButton = sf::RectangleShape(secondButtonSize);
    secondButton.setPosition(secondButtonPos);
    secondButton.setFillColor(sf::Color::Green);
    secondButton.setOutlineColor(sf::Color::White);
    m_buttons.insert(std::make_pair("second", secondButton));
    auto thirdButton = sf::RectangleShape(thirdButtonSize);
    thirdButton.setPosition(thirdButtonPos);
    thirdButton.setFillColor(sf::Color::Blue);
    thirdButton.setOutlineColor(sf::Color::White);
    m_buttons.insert(std::make_pair("third", thirdButton));
}

void Scene_Play::update()
{
    
    m_entityManager.update();
}

void Scene_Play::sDoAction(const Action & action)
{
    if (action.type() == "START")
    {
        if (action.name() == "QUIT") { onEnd(); }
    }
    else if (action.type() == "END")
    {

    }
}

void Scene_Play::sDoActionMouse(const Action & action, const Vec2 & pos)
{
    if (action.type() == "START")
    {
        // check if the mouse is over a button
        for (auto & button : m_buttons)
        {
            if (button.second.getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y)))
            {
                if (button.first == "first")
                {
                    m_buttonSelected = 0;
                }
                else if (button.first == "second")
                {
                    m_buttonSelected = 1;
                }
                else if (button.first == "third")
                {
                    m_buttonSelected = 2;
                }
            }
        }
    }
}

void Scene_Play::onEnd()
{
    
    m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
}

void Scene_Play::sRender()
{
    // color the background darker so you know that the game is paused
    if (!m_paused) { m_game->window().clear(sf::Color(100, 100, 255)); }
    else { m_game->window().clear(sf::Color(50, 50, 150)); }

    float windowCenterX = m_game->window().getSize().x / 2.0f;
    float windowCenterY = m_game->window().getSize().y / 2.0f;

    // TODO: draw buttons
    for (auto & button : m_buttons)
    {
        if (
            (m_buttonSelected == 0 && button.first == "first")
            || (m_buttonSelected == 1 && button.first == "second")
            || (m_buttonSelected == 2 && button.first == "third")
        )
        {
            button.second.setOutlineThickness(2);
        }
        else
        {
            button.second.setOutlineThickness(0);
        }
        m_game->window().draw(button.second);
    }
}

void Scene_Play::sReceive(std::string & message)
{
    // do stuff
    std::cout << "Received message in play: " << message << std::endl;
    // json parse message
    m_jsonParser.parse(message);
    std::string type = m_jsonParser.get("type");
    if (type == "")
    {
    }
}

void Scene_Play::sSend(std::string & message)
{
    m_game->sendNetworkMessage(message);
}