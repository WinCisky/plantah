#pragma once

#include "Scene.h"
#include "Assets.h"
#include "NetworkManager.h"

#include <memory>

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{
private:
    // const std::string IP = "planta.opentrust.it";
    const std::string IP = "localhost";
    // const int PORT = 8080;
    const int PORT = 6666;

protected:
    sf::RenderWindow m_window;
    Assets m_assets;
    std::string m_path;
    std::string m_currentScene;
    SceneMap m_sceneMap;
    size_t m_simulationSpeed = 1;
    bool m_running = true;
    NetworkManager m_networkManager;

    void init(const std::string & path);
    void update();
    void updateNetwork();

    void sUserInput();

    std::shared_ptr<Scene> currentScene();

public:

    GameEngine(const std::string & path);

    void changeScene(const std::string & sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

    void quit();
    void run();

    sf::RenderWindow & window();
    const Assets& assets() const;
    bool isRunning();
    void sendNetworkMessage(std::string & message);
    std::string getPath() const;
};