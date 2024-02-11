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
    , m_playText(m_game->assets().getFont("assets/tech.ttf"), "Play", 24)
{
    init();
}

void Scene_Play::init()
{
    registerAction(sf::Keyboard::Key::Escape, "QUIT");
    registerActionMouse(sf::Mouse::Button::Left, "CLICK");

    m_screenSize = m_game->window().getSize();
    m_screenQuarter = m_screenSize / 4u;
    m_quarterSixth = m_screenQuarter / 6u;
    m_isLandscape = m_screenSize.x > m_screenSize.y;

    // assign buttons size and position
    placeButtons();
    m_buttonSelected = -1;
    m_buttonConfirmed = -1;

    // create players
    m_players.push_back(Player());
    m_players.push_back(Player());
}

void Scene_Play::placeButtons()
{
    // place first bottom left
    auto firstButtonSize = m_isLandscape 
        ? sf::Vector2f(100, 100) // TODO 
        : sf::Vector2f(m_screenQuarter.x, m_screenQuarter.x);
    auto firstButtonPos = m_isLandscape 
        ? sf::Vector2f(100, 100) // TODO
        : sf::Vector2f(
            m_quarterSixth.x, 
            m_screenSize.y - m_quarterSixth.y - firstButtonSize.x
        );
        
    // place second bottom center
    auto secondButtonSize = firstButtonSize;
    auto secondButtonPos = m_isLandscape
        ? sf::Vector2f(100, 100) // TODO
        : sf::Vector2f(
            (m_screenSize.x / 2.0) - (secondButtonSize.x / 2.0), 
            m_screenSize.y - m_quarterSixth.y - secondButtonSize.x
        );
    // place third bottom right
    auto thirdButtonSize = firstButtonSize;
    auto thirdButtonPos = m_isLandscape
        ? sf::Vector2f(100, 100) // TODO
        : sf::Vector2f(
            m_screenSize.x - m_quarterSixth.x - thirdButtonSize.x, 
            m_screenSize.y - m_quarterSixth.y - thirdButtonSize.x
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

void Scene_Play::sendChoice(short index)
{
    // show content of m_choices
    std::cout << "Choices: ";
    for (auto & choice : m_choices)
    {
        std::cout << choice << " ";
    }
    std::cout << std::endl;
    return;
    
    // send lobby and choice
    std::string messageTemplate = "{ \"type\": \"pick\", \"lobby\": %d, \"choice\": %d }";
    std::string message = std::string(
        messageTemplate.c_str(), 
        m_lobby, 
        m_choices[index]
    );
    sSend(message);
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
                    // get choiche with index 0
                    std::cout << "Choice: " << m_choices[0] << std::endl;
                    sendChoice(0);

                }
                else if (button.first == "second")
                {
                    m_buttonSelected = 1;
                    sendChoice(1);
                }
                else if (button.first == "third")
                {
                    m_buttonSelected = 2;
                    sendChoice(2);
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
        if (m_choices[0] < 0) continue;
        // selected as white
        button.second.setOutlineColor(sf::Color::White);
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
        // confirmed as green
        if (
            (m_buttonConfirmed == 0 && button.first == "first")
            || (m_buttonConfirmed == 1 && button.first == "second")
            || (m_buttonConfirmed == 2 && button.first == "third")
        )
        {
            button.second.setOutlineColor(sf::Color::Green);
            button.second.setOutlineThickness(2);
        }
        m_game->window().draw(button.second);
    }

    // TODO: draw trees
    // TODO: draw resources
    auto remainingSize = m_isLandscape
        ? sf::Vector2f(m_screenSize.x, m_screenSize.y) // TODO
        : sf::Vector2f(m_screenSize.x, m_screenSize.y - (m_screenQuarter.x + (2 * m_quarterSixth.x)));
    // 70 % to gameplay and 30 % to resources
    auto gameplaySize = m_isLandscape
        ? sf::Vector2f(m_screenSize.x, m_screenSize.y) // TODO
        : sf::Vector2f(remainingSize.x * 0.7, remainingSize.y);
    auto resourcesSize = m_isLandscape
        ? sf::Vector2f(m_screenSize.x, m_screenSize.y) // TODO
        : sf::Vector2f(remainingSize.x * 0.3, remainingSize.y);
    

    // print trees and resources
    auto posPlayer1 = m_isLandscape
        ? sf::Vector2f(100, 100) // TODO
        : sf::Vector2f(0, gameplaySize.y - 20);
    auto posPlayer2 = m_isLandscape
        ? sf::Vector2f(100, 100) // TODO
        : sf::Vector2f(gameplaySize.x / 2, gameplaySize.y - 20);
    auto posResource1 = m_isLandscape
        ? sf::Vector2f(100, 100) // TODO
        : sf::Vector2f(gameplaySize.x, resourcesSize.y - 20);
    auto posResource2 = m_isLandscape
        ? sf::Vector2f(100, 100) // TODO
        : sf::Vector2f(gameplaySize.x + (resourcesSize.x / 3), resourcesSize.y - 20);
    auto posResource3 = m_isLandscape
        ? sf::Vector2f(100, 100) // TODO
        : sf::Vector2f(gameplaySize.x + (2 * (resourcesSize.x / 3)), resourcesSize.y - 20);
    
    m_playText.setString("1");
    m_playText.setPosition(posPlayer1);
    m_game->window().draw(m_playText);
    m_playText.setString("2");
    m_playText.setPosition(posPlayer2);
    m_game->window().draw(m_playText);
    m_playText.setString("3");
    m_playText.setPosition(posResource1);
    m_game->window().draw(m_playText);
    m_playText.setString("4");
    m_playText.setPosition(posResource2);
    m_game->window().draw(m_playText);
    m_playText.setString("5");
    m_playText.setPosition(posResource3);
    m_game->window().draw(m_playText);
}

void Scene_Play::sReceive(std::string & message)
{
    std::cout << "Received message in play: " << message << std::endl;
    m_jsonParser.parse(message);
    std::string type = m_jsonParser.get("type");
    if (type == "choices")
    {
        m_buttonSelected = -1;
        m_buttonConfirmed = -1;
        auto choices = m_jsonParser.getArray("choices");
        std::cout << "Choices: " << choices << std::endl;
        // [0,4,3] -> 0, 4, 3
        choices = choices.substr(1, choices.size() - 2);
        std::string choice;
        short index = 0;
        for (auto & c : choices)
        {
            if (c == ',')
            {
                m_choices[index++] = std::stoi(choice);
                choice = "";
            }
            else
            {
                choice += c;
            }
        }
        m_choices[index] = std::stoi(choice);     
        // do stuff
    }
    else if (type == "lobbyUpdated")
    {
        auto playersChoices = m_jsonParser.get("playersChoices");
        std::cout << "Players choices: " << playersChoices << std::endl;
        // do stuff
    }
}

void Scene_Play::sSend(std::string & message)
{
    m_game->sendNetworkMessage(message);
}