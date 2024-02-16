#include "Scene_GameOver.h"
#include "GameEngine.h"
#include "Scene_Menu.h"

Scene_GameOver::Scene_GameOver(GameEngine * gameEngine, int winner)
    : Scene(gameEngine)
    , m_title("Game Over")
    // , m_menuText(sf::Text(m_game->assets().getFont("assets/tech.ttf"), "", 20))
{
    m_isWinner = winner == m_game->playerId;

    init();
}

void Scene_GameOver::init()
{
    registerActionMouse(sf::Mouse::Button::Left, "CLICK");

    m_menuText.setFont(m_game->assets().getFont("assets/tech.ttf"));
    m_menuText.setCharacterSize(20);

    auto screen = m_game->window().getSize();
    m_buttonContinue.setSize(sf::Vector2f(100, 50));
    m_buttonContinue.setFillColor(sf::Color(50, 50, 50));
    m_buttonContinue.setPosition(sf::Vector2f(screen.x / 2 - 50, (3 * screen.y) / 4));
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
    if (action.type() == "START")
    {
        auto firstButtonPos = m_buttonContinue.getPosition();
        auto buttonSize = m_buttonContinue.getSize();

        if (pos.x > firstButtonPos.x && pos.x < firstButtonPos.x + buttonSize.x &&
            pos.y > firstButtonPos.y && pos.y < firstButtonPos.y + buttonSize.y)
        {
            m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game), true);
        }
    }
}

void Scene_GameOver::sRender()
{
    m_menuText.setOrigin(sf::Vector2f(0, 0));

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

    // auto text = sf::Text{ "Test 1234", font };
    // text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
    // text.setPosition(rectangle.getPosition() + (rectangle.getSize() / 2.f));

    m_game->window().draw(m_buttonContinue);
    m_menuText.setString(CONTINUE);
    m_menuText.setOrigin(m_menuText.getGlobalBounds().getSize() / 2.f + m_menuText.getLocalBounds().getPosition());

    auto textSize = m_menuText.getGlobalBounds();
    m_menuText.setPosition(m_buttonContinue.getPosition() + (m_buttonContinue.getSize() / 2.f));
    m_game->window().draw(m_menuText);
}