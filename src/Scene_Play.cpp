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

    m_playText.setFont(m_game->assets().getFont("assets/tech.ttf"));
    m_playText.setCharacterSize(20);

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
    firstButton.setFillColor(sf::Color(50, 50, 50));
    firstButton.setOutlineColor(sf::Color(255, 255, 255));
    m_buttons.insert(std::make_pair("first", firstButton));
    auto secondButton = sf::RectangleShape(secondButtonSize);
    secondButton.setPosition(secondButtonPos);
    secondButton.setFillColor(sf::Color(50, 50, 50));
    secondButton.setOutlineColor(sf::Color(255, 255, 255));
    m_buttons.insert(std::make_pair("second", secondButton));
    auto thirdButton = sf::RectangleShape(thirdButtonSize);
    thirdButton.setPosition(thirdButtonPos);
    thirdButton.setFillColor(sf::Color(50, 50, 50));
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
    // send lobby and choice
    char buffer[512];
    snprintf(
        buffer, 
        sizeof(buffer), 
        "{ \"type\": \"pick\", \"lobby\": %d, \"choice\": %d }", 
        m_lobby, 
        m_choices[index]
    );
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
    m_game->window().clear();

    drawButtons();

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
    drawPlayers(gameplaySize);
    drawResources(resourcesSize, gameplaySize);
    drawWeather(gameplaySize);
    drawCountdown();
}

void Scene_Play::drawWeather(sf::Vector2f gamePlaySize)
{
    if (m_weather == "") { return; }
    // divide width between players
    int playerCount = m_players.size();
    // +1 to leave space and +1 for weather
    float playerWidth = gamePlaySize.x / (playerCount + 2);
    // +1 to leave space
    float plantHeight = gamePlaySize.y / (WINNING_HEIGHT + 1);
    float paddingWidth = playerWidth / (2 * playerCount);
    float paddingHeight = plantHeight / 2;

    // draw weather top right
    auto weatherSize = m_isLandscape
        ? sf::Vector2f(100, 100) // TODO
        : sf::Vector2f(plantHeight, plantHeight);
    auto weatherPos = m_isLandscape
        ? sf::Vector2f(100, 100) // TODO
        : sf::Vector2f(gamePlaySize.x - weatherSize.x - paddingWidth, paddingHeight);
    auto weatherSprite = sf::Sprite();
    weatherSprite.setTexture(m_game->assets().getTexture(m_textureWeatherAssociation.at(m_weather)));
    weatherSprite.setPosition(weatherPos);
    weatherSprite.setScale(
        weatherSize.x / weatherSprite.getGlobalBounds().width,
        weatherSize.y / weatherSprite.getGlobalBounds().height
    );
    m_game->window().draw(weatherSprite);
}

void Scene_Play::drawButtons()
{
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

        sf::Sprite sprite;

        if (button.first == "first")
        {
            sprite.setTexture(m_game->assets().getTexture(m_textureAssociation.at(m_choices[0])));
            sprite.setPosition(button.second.getPosition());
            sprite.setScale(
                button.second.getSize().x / sprite.getGlobalBounds().width,
                button.second.getSize().y / sprite.getGlobalBounds().height
            );
            m_game->window().draw(sprite);
        }
        else if (button.first == "second")
        {
            sprite.setTexture(m_game->assets().getTexture(m_textureAssociation.at(m_choices[1])));
            sprite.setPosition(button.second.getPosition());
            sprite.setScale(
                button.second.getSize().x / sprite.getGlobalBounds().width,
                button.second.getSize().y / sprite.getGlobalBounds().height
            );
            m_game->window().draw(sprite);
        }
        else if (button.first == "third")
        {
            sprite.setTexture(m_game->assets().getTexture(m_textureAssociation.at(m_choices[2])));
            sprite.setPosition(button.second.getPosition());
            sprite.setScale(
                button.second.getSize().x / sprite.getGlobalBounds().width,
                button.second.getSize().y / sprite.getGlobalBounds().height
            );
            m_game->window().draw(sprite);
        }
    }
}

void Scene_Play::drawPlayers(sf::Vector2f gamePlaySize)
{
    // divide width between players
    int playerCount = m_players.size();
    // +1 to leave space and +1 for weather
    float playerWidth = gamePlaySize.x / (playerCount + 2);
    // +1 to leave space
    float plantHeight = gamePlaySize.y / (WINNING_HEIGHT + 1);
    float paddingWidth = playerWidth / 2;
    float paddingHeight = plantHeight / 2;


    // draw rectangle with white outline for the whole game play area
    sf::RectangleShape rectangle(sf::Vector2f(
        gamePlaySize.x - (2 * paddingWidth), 
        gamePlaySize.y - (2 * paddingHeight))
    );
    rectangle.setPosition(paddingWidth, paddingHeight);
    rectangle.setFillColor(sf::Color(50, 50, 50, 255));
    rectangle.setOutlineColor(sf::Color(255, 255, 255));
    rectangle.setOutlineThickness(1);
    m_game->window().draw(rectangle);
    // draw growth rows
    for (int i = 1; i <= (WINNING_HEIGHT - 1); i++)
    {
        sf::RectangleShape row(sf::Vector2f(gamePlaySize.x - (2 * paddingWidth), 1));
        row.setPosition(paddingWidth, (i * (gamePlaySize.y / (WINNING_HEIGHT + 1))) + paddingHeight);
        row.setFillColor(sf::Color(255, 255, 255));
        m_game->window().draw(row);
    }
    sf::RectangleShape column(sf::Vector2f(1, gamePlaySize.y - (2 * paddingHeight)));
    // draw players dividers columns
    for (int i = 1; i <= ((playerCount +1) * 2); i++)
    {
        column.setPosition(i * (playerWidth / 2), paddingHeight);
        column.setFillColor(sf::Color(255, 255, 255));
        m_game->window().draw(column);
    }


    for (int i = 0; i < playerCount; i++)
    {
        auto player = m_players[i];
        auto basePos = m_isLandscape
            ? sf::Vector2f(100, 100) // TODO
            : sf::Vector2f(
                ((i + 1) * playerWidth) - (plantHeight / 2), 
                gamePlaySize.y - paddingHeight
            );
        
        // draw the plant
        sf::Sprite sprite;
        sprite.setTexture(m_game->assets().getTexture("bud"));
        sprite.setScale(
            plantHeight / sprite.getGlobalBounds().width,
            plantHeight / sprite.getGlobalBounds().height
        );

        for (int j = 1; j <= player.HEIGHT; j++)
        {
            sprite.setPosition(basePos.x, basePos.y - (j * plantHeight));
            if (j == player.HEIGHT)
            {
                sprite.setTexture(m_game->assets().getTexture("bud"));
            }
            else
            {
                sprite.setTexture(m_game->assets().getTexture("stem"));
            }
            m_game->window().draw(sprite);
        }
    }
}

void Scene_Play::drawResources(sf::Vector2f resourcesSize, sf::Vector2f gameplaySize) 
{
    // +1 to leave space and +1 for icon
    float resourceHeight = resourcesSize.y / (MAX_RESOURCES + 2);
    float paddingHeight = resourceHeight / 2;
    // +1 to leave space
    float resourceWidth = resourcesSize.x / 4;
    float paddingWidth = resourceWidth / 2;

    // // draw grid
    // sf::RectangleShape resourcesArea(sf::Vector2f(
    //     resourcesSize.x - (2 * paddingWidth), 
    //     resourcesSize.y - (2* paddingHeight)
    // ));
    // resourcesArea.setPosition(gameplaySize.x + paddingWidth, paddingHeight);
    // resourcesArea.setFillColor(sf::Color(50, 50, 50));
    // resourcesArea.setOutlineColor(sf::Color(255, 255, 255));
    // resourcesArea.setOutlineThickness(1);
    // m_game->window().draw(resourcesArea);

    // min between width and height
    float circleSize = m_isLandscape
        ? 100 // TODO
        : std::min(resourceHeight / 3.14, resourceWidth / 3.14);
    sf::CircleShape shape(circleSize);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < MAX_RESOURCES; j++)
        {
            auto pos = m_isLandscape
                ? sf::Vector2f(100, 100) // TODO
                : sf::Vector2f(
                    gameplaySize.x + (i * resourceWidth) + resourceWidth - circleSize, 
                    resourcesSize.y - (j * resourceHeight) - circleSize * 2 - paddingHeight
                );
            shape.setPosition(pos);
            shape.setFillColor(sf::Color(50, 50, 50));
            if (m_players[m_playerIndex].RESOURCES[i] > j)
            {
                shape.setFillColor(sf::Color(255, 255, 255));
            }
            shape.setOutlineColor(sf::Color(255, 255, 255));
            shape.setOutlineThickness(1);
            m_game->window().draw(shape);

            // // draw grid
            // sf::RectangleShape row(sf::Vector2f(resourcesSize.x - (2 * paddingWidth), 1));
            // row.setPosition(
            //     gameplaySize.x + paddingWidth, 
            //     resourcesSize.y - ((j * resourceHeight) + paddingHeight)
            // );
            // row.setFillColor(sf::Color(255, 255, 255));
            // m_game->window().draw(row);
            // row.setPosition(
            //     gameplaySize.x + paddingWidth, 
            //     resourcesSize.y - (((j + 1) * resourceHeight) + paddingHeight)
            // );
            // row.setFillColor(sf::Color(255, 255, 255));
            // m_game->window().draw(row);

        }
        // // draw grid
        // sf::RectangleShape column(sf::Vector2f(1, resourcesSize.y - (2 * paddingHeight)));
        // column.setPosition(gameplaySize.x + (i * resourceWidth) + paddingWidth, paddingHeight);
        // column.setFillColor(sf::Color(255, 255, 255));
        // m_game->window().draw(column);
    }

    // draw resources icons
    auto iconsOrder = std::vector<std::string>{ "lamp", "water", "fertilizer" };
    auto scaleMultiplier = m_isLandscape ? 1 : 1.5f;
    auto sizeDiff = m_isLandscape ? 0 : (resourceWidth * scaleMultiplier) - resourceWidth;
    for (int i = 0; i < 3; i++)
    {
        sf::Sprite sprite;
        sprite.setTexture(m_game->assets().getTexture(iconsOrder[i]));
        sprite.setScale(
            (resourceWidth * scaleMultiplier) / sprite.getGlobalBounds().width,
            (resourceWidth * scaleMultiplier) / sprite.getGlobalBounds().height
        );
        sprite.setPosition(m_isLandscape
            ? sf::Vector2f(100, 100) // TODO
            : sf::Vector2f(
                gameplaySize.x + (i * resourceWidth) + paddingWidth - sizeDiff / 2, 
                paddingHeight + resourceHeight - resourceWidth - sizeDiff / 2
            )
        );
        m_game->window().draw(sprite);
    }
}

void Scene_Play::drawCountdown()
{
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
            if (resource <= RESOURCES_TRESHOLD)
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
    m_game->sendNetworkMessage(message);
}
