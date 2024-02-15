#include "Scene_Play.h"
#include "Scene_Menu.h"
#include "Scene_GameOver.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"

#include <iostream>

Scene_Play::Scene_Play(GameEngine * gameEngine, short lobbyId, std::vector<int> players)
    : Scene(gameEngine)
    , m_playText(m_game->assets().getFont("assets/tech.ttf"), "Play", 20)
    , m_lobby(lobbyId)
{
    int index = 0;
    for (auto & player : players)
    {
        Player p;
        p.ID = player;
        m_players.push_back(p);
        if (player == m_game->playerId)
        {
            m_playerIndex = index;
        }
        index++;
    }

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

    m_timerRectangle = sf::RectangleShape(sf::Vector2f(m_screenSize.x, 3));
    m_timerRectangle.setPosition(sf::Vector2f(0, m_screenSize.y - 3));
    m_timerRectangle.setFillColor(sf::Color::Green);
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
    firstButton.setOutlineColor(sf::Color(255, 255, 255));
    m_buttons.insert(std::make_pair("first", firstButton));
    auto secondButton = sf::RectangleShape(secondButtonSize);
    secondButton.setPosition(secondButtonPos);
    secondButton.setFillColor(sf::Color::Green);
    secondButton.setOutlineColor(sf::Color(255, 255, 255));
    m_buttons.insert(std::make_pair("second", secondButton));
    auto thirdButton = sf::RectangleShape(thirdButtonSize);
    thirdButton.setPosition(thirdButtonPos);
    thirdButton.setFillColor(sf::Color::Blue);
    thirdButton.setOutlineColor(sf::Color(255, 255, 255));
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
    // return;
    
    // send lobby and choice
    char buffer[512];
    snprintf(buffer, sizeof(buffer), "{ \"type\": \"pick\", \"lobby\": %d, \"choice\": %d }", m_lobby, m_choices[index]);
    std::string message = std::string(buffer);
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
        button.second.setOutlineColor(sf::Color(255, 255, 255));
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
            (m_choices[0] == m_buttonConfirmed && button.first == "first")
            || (m_choices[1] == m_buttonConfirmed && button.first == "second")
            || (m_choices[2] == m_buttonConfirmed && button.first == "third")
        )
        {
            button.second.setOutlineColor(sf::Color::Green);
            button.second.setOutlineThickness(2);
        }
        m_game->window().draw(button.second);

        if (button.first == "first")
        {
            m_playText.setString(std::to_string(m_choices[0]));
            // center the text in the button
            m_playText.setPosition(
                sf::Vector2f(
                    button.second.getPosition().x + (button.second.getSize().x / 2.0) - 10,
                    button.second.getPosition().y + (button.second.getSize().y / 2.0) - 10
                )
            );
            m_game->window().draw(m_playText);
        }
        else if (button.first == "second")
        {
            m_playText.setString(std::to_string(m_choices[1]));
            // center the text in the button
            m_playText.setPosition(
                sf::Vector2f(
                    button.second.getPosition().x + (button.second.getSize().x / 2.0) - 10,
                    button.second.getPosition().y + (button.second.getSize().y / 2.0) - 10
                )
            );
            m_game->window().draw(m_playText);
        }
        else if (button.first == "third")
        {
            m_playText.setString(std::to_string(m_choices[2]));
            // center the text in the button
            m_playText.setPosition(
                sf::Vector2f(
                    button.second.getPosition().x + (button.second.getSize().x / 2.0) - 10,
                    button.second.getPosition().y + (button.second.getSize().y / 2.0) - 10
                )
            );
            m_game->window().draw(m_playText);
        }
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
    
    auto otherPlayer = m_playerIndex == 0 ? 1 : 0;    
    m_playText.setString(std::to_string(m_players[m_playerIndex].HEIGHT));
    m_playText.setPosition(posPlayer1);
    m_game->window().draw(m_playText);
    m_playText.setString(std::to_string(m_players[otherPlayer].HEIGHT));
    m_playText.setPosition(posPlayer2);
    m_game->window().draw(m_playText);
    m_playText.setString(std::to_string(m_players[m_playerIndex].RESOURCES[0]));
    m_playText.setPosition(posResource1);
    m_game->window().draw(m_playText);
    m_playText.setString(std::to_string(m_players[m_playerIndex].RESOURCES[1]));
    m_playText.setPosition(posResource2);
    m_game->window().draw(m_playText);
    m_playText.setString(std::to_string(m_players[m_playerIndex].RESOURCES[2]));
    m_playText.setPosition(posResource3);
    m_game->window().draw(m_playText);

    // draw remaining time
    auto time = m_clock.getElapsedTime();
    auto remainingTime = 4.8f - time.asSeconds();
    if (remainingTime < 0)
    {
        remainingTime = 0;
    }
    
    m_timerRectangle.setSize(sf::Vector2f((remainingTime / 4.8f) * m_screenSize.x, 3));
    m_game->window().draw(m_timerRectangle);
}

void Scene_Play::incrementPlayerResource(int playerId, short resource, short increment)
{
    if ((m_players[playerId].RESOURCES[resource] + increment) >= MAX_RESOURCES)
    {
        m_players[playerId].RESOURCES[resource] = MAX_RESOURCES;
    }
    else
    {
        m_players[playerId].RESOURCES[resource] += increment;
    }
}

void Scene_Play::decrementPlayerResource(int playerId, short resource, short decrement)
{
    if ((m_players[playerId].RESOURCES[resource] - decrement) <= 0)
    {
        m_players[playerId].RESOURCES[resource] = 0;
    }
    else
    {
        m_players[playerId].RESOURCES[resource] -= decrement;
    }
}

void Scene_Play::updatePlayers(std::vector<PlayerChoice> & choices)
{
    for (auto & choice : choices)
    {
        int playerIndex = -1;
        for (int i = 0; i < m_players.size(); i++)
        {
            if (m_players[i].ID == choice.m_playerId)
            {
                playerIndex = i;
                break;
            }
        }
        if (playerIndex == -1) { continue; }
        int randomResource;
        switch (choice.m_choice)
        {
        case 0: // increment resource 1
            incrementPlayerResource(playerIndex, 0, RESOURCE_INCREMENT);
            break;
        case 1: // increment resource 2
            incrementPlayerResource(playerIndex, 1, RESOURCE_INCREMENT);
            break;
        case 2: // increment resource 3
            incrementPlayerResource(playerIndex, 2, RESOURCE_INCREMENT);
            break;
        case 3: // increment trinity
            incrementPlayerResource(playerIndex, 0, TRINITY_INCREMENT);
            incrementPlayerResource(playerIndex, 1, TRINITY_INCREMENT);
            incrementPlayerResource(playerIndex, 2, TRINITY_INCREMENT);
            break;
        case 4: // decrement resource 1
            decrementPlayerResource(playerIndex, 0, RESOURCE_DECREMENT);
            break;
        case 5: // decrement resource 2
            decrementPlayerResource(playerIndex, 1, RESOURCE_DECREMENT);
            break;
        case 6: // decrement resource 3
            decrementPlayerResource(playerIndex, 2, RESOURCE_DECREMENT);
            break;
        case 7: // decrement trinity
            decrementPlayerResource(playerIndex, 0, TRINITY_DECREMENT);
            decrementPlayerResource(playerIndex, 1, TRINITY_DECREMENT);
            decrementPlayerResource(playerIndex, 2, TRINITY_DECREMENT);
            break;
        }

        // return;
        // std::cout << "Player " << m_players[playerIndex].ID << " resources: ";
        // for (auto & resource : m_players[playerIndex].RESOURCES)
        // {
        //     std::cout << resource << " ";
        // }
        // std::cout << std::endl;

        // weather
        if (m_weather == "sunny")
        {
            incrementPlayerResource(playerIndex, 0, WEATHER_INCREMENT);
        }
        else if (m_weather == "rainy")
        {
            incrementPlayerResource(playerIndex, 1, WEATHER_INCREMENT);
        }
        else if (m_weather == "moon")
        {
            incrementPlayerResource(playerIndex, 2, WEATHER_INCREMENT);
        }

        // growth if all resources are above treshold
        bool allResourcesAboveTreshold = true;
        for (auto & resource : m_players[playerIndex].RESOURCES)
        {
            if (resource < RESOURCES_TRESHOLD)
            {
                allResourcesAboveTreshold = false;
                break;
            }
        }
        if (allResourcesAboveTreshold)
        {
            m_players[playerIndex].HEIGHT += 1;
        }

        // decrement all resources by 1
        decrementPlayerResource(playerIndex, 0, GROWTH_DECREMENT);
        decrementPlayerResource(playerIndex, 1, GROWTH_DECREMENT);
        decrementPlayerResource(playerIndex, 2, GROWTH_DECREMENT);
    }

    // prints every user's resources
    for (auto & player : m_players)
    {
        std::cout << "Player " << player.ID << " resources: ";
        for (auto & resource : player.RESOURCES)
        {
            std::cout << resource << " ";
        }
        std::cout << std::endl;
    }
}


void Scene_Play::sReceive(std::string & message)
{
    std::cout << "Received message in play: " << message << std::endl;
    m_jsonParser.parse(message);
    std::string type = m_jsonParser.get("type");
    if (type == "choices")
    {
        m_clock.restart();
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
        m_weather = m_jsonParser.get("weather");
    }
    else if (type == "lobbyUpdated")
    {
        m_clock.restart();
        std::vector<PlayerChoice> playerChoices;
        SimpleJsonParser lobbyParser;
        auto playersChoices = m_jsonParser.getArray("playersChoices");
        // split the array into individual choices using brackets { ... }, { ... }
        std::vector<std::string> choices;
        std::string choice;
        for (auto & c : playersChoices)
        {
            choice += c;
            if (c == '{')
            {
                choice = "";
            }
            else if (c == '}')
            {
                choices.push_back(choice);
            }
        }
        // for each choice, parse it and get the player id and pick
        for (auto & c : choices)
        {
            lobbyParser.parse(c);
            std::string playerId = lobbyParser.getNumber("id");
            std::string playerPick = lobbyParser.getNumber("pick");
            short id = std::stoi(playerId);
            short pick = std::stoi(playerPick);
            playerChoices.push_back({ id, pick });

            if (id == m_game->playerId)
            {
                m_buttonConfirmed = pick;
            }
        }
        updatePlayers(playerChoices);
    }
    else if (type == "endGame")
    {
        int winnerId = std::stoi(m_jsonParser.getNumber("winner"));
        m_game->changeScene("GAMEOVER", std::make_shared<Scene_GameOver>(m_game, winnerId));
    }
}

void Scene_Play::sSend(std::string & message)
{
    // std::cout << "Sending message in play: " << message << std::endl;
    m_game->sendNetworkMessage(message);
}
