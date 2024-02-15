#include "Scene_GameOver.h"
#include "GameEngine.h"

Scene_GameOver::Scene_GameOver(GameEngine * gameEngine, int winner)
    : Scene(gameEngine)
    , m_title("Game Over")
    , m_menuText(sf::Text(m_game->assets().getFont("assets/tech.ttf"), "", 20))
{
    m_isWinner = winner == m_game->playerId;

    init();
}

void Scene_GameOver::init()
{
    
}

void Scene_GameOver::update()
{

}

void Scene_GameOver::onEnd()
{

}

void Scene_GameOver::sDoAction(const Action & action)
{

}

void Scene_GameOver::sDoActionMouse(const Action & action, const Vec2 & pos)
{

}

void Scene_GameOver::sRender()
{
    m_game->window().clear(sf::Color::Black);
    m_menuText.setString(m_title);
    m_menuText.setPosition(sf::Vector2f(100, 200));
    m_game->window().draw(m_menuText);

    if (m_isWinner)
    {
        m_menuText.setString(VICTORY);
        m_menuText.setPosition(sf::Vector2f(100, 300));
        m_game->window().draw(m_menuText);
    }
    else
    {
        m_menuText.setString(DEFEAT);
        m_menuText.setPosition(sf::Vector2f(100, 300));
        m_game->window().draw(m_menuText);
    }
    
    m_game->window().display();
}