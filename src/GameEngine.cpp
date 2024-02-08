#include "GameEngine.h"
#include "Assets.h"
#include "Scene_Play.h"
#include "Scene_Menu.h"

#include <iostream>

GameEngine::GameEngine(const std::string & path)
    : m_networkManager(NetworkManager(IP, PORT))
{
    init(path);
}

void GameEngine::init(const std::string & path)
{
    m_assets.loadFromFile(path);

    auto videoMode = sf::VideoMode(sf::Vector2u(360, 640));
    sf::VideoMode screen(videoMode);
    m_window.create(screen, "Definetly not Mario");
    m_window.setFramerateLimit(30);

    changeScene("MENU", std::make_shared<Scene_Menu>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
    return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning()
{
    return m_running & m_window.isOpen();
}

sf::RenderWindow & GameEngine::window()
{
    return m_window;
}

void GameEngine::run()
{
    while (isRunning())
    {
        updateNetwork();
        update();
    }
}

void GameEngine::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Key::X)
            {
                std::cout << "screenshot saved to " << "test.png" << std::endl;
                sf::Texture texture;
                bool isTextureCreated = texture.create(sf::Vector2u(m_window.getSize().x, m_window.getSize().y));
                if (!isTextureCreated)
                {
                    std::cout << "Failed to create texture" << std::endl;
                    continue;
                }
                texture.update(m_window);
                if (texture.copyToImage().saveToFile("test.png"))
                {
                    std::cout << "screenshot saved to " << "test.png" << std::endl;
                }
            }
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            // if the current scene does not have an action associated with this key, skip the event
            if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
            {
                continue;
            }

            // determine start or end action by whether it was key pres or release
            const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

            // look up the action and send the action to the scene
            currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
        }

        if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
        {
            // if the current scene does not have an action associated with this key, skip the event
            if (currentScene()->getActionMapMouse().find(event.mouseButton.button) == currentScene()->getActionMapMouse().end())
            {
                continue;
            }

            // determine start or end action by whether it was key pres or release
            const std::string actionType = (event.type == sf::Event::MouseButtonPressed) ? "START" : "END";

            auto mousePos = sf::Mouse::getPosition(m_window);
            // look up the action and send the action to the scene
            currentScene()->doActionMouse(
                Action(currentScene()->getActionMapMouse().at(event.mouseButton.button), actionType), 
                Vec2(mousePos.x, mousePos.y)
            );
        }

        if (event.type == sf::Event::Closed)
        {
            quit();
        }
    }
}

void GameEngine::changeScene(const std::string & sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
    m_sceneMap[sceneName] = scene;
    m_currentScene = sceneName;
}

void GameEngine::update()
{
    m_window.clear();
	sUserInput();
    currentScene()->update();
    currentScene()->sRender();
    m_window.display();
}

void GameEngine::updateNetwork()
{
    if (m_networkManager.isConnected())
    {
        std::string message = m_networkManager.sReceive();
        if (message != "")
        {
            // std::cout << "Received message: " << message << std::endl;
            // let scene handle message
            currentScene()->sReceive(message);
        }
    }
}

void GameEngine::sendNetworkMessage(std::string & message)
{
    m_networkManager.sSend(message);
}

const Assets& GameEngine::assets() const
{
	return m_assets;
}

void GameEngine::quit()
{
    m_running = false;
}
