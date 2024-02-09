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
    registerAction(sf::Keyboard::Key::P, "PAUSE");
    registerAction(sf::Keyboard::Key::Escape, "QUIT");
    registerAction(sf::Keyboard::Key::T, "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::Key::C, "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::Key::G, "TOGGLE_GRID");

    // TODO: Register all other fameplay Actions

    // m_gridText.setCharacterSize(12);
    // m_gridText.setFont(m_game->assets().getFont("assets/tech.ttf"));

    // loadLevel(levelPath);
}

void Scene_Play::update()
{
    m_entityManager.update();

    // TODO: implement pause functionality
    // sAnimation();
    // sRender();
}

void Scene_Play::sDoAction(const Action & action)
{
    // if (action.type() == "START")
    // {
    //     if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
    //     else if (action.name() == "TOGGLE_COLLISION") { m_drawCollisions = !m_drawCollisions; }
    //     else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
    //     else if (action.name() == "PAUSE") { setPaused(!m_paused); }
    //     else if (action.name() == "QUIT") { onEnd(); }
    // }
    // else if (action.type() == "END")
    // {

    // }
}

void Scene_Play::sDoActionMouse(const Action & action, const Vec2 & pos)
{

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
}

void Scene_Play::sReceive(std::string & message)
{
    // do stuff
    std::cout << "Received message in lobby: " << message << std::endl;
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