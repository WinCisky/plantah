#pragma once

#include "Scene.h"
#include <map>
#include <memory>
#include <deque>

#include "EntityManager.h"
#include "SimpleJsonParser.h"

class Scene_Lobby : public Scene
{
private:
    const u_short MAX_DOTS = 3;
protected:

    u_short m_dots;
    std::string m_title;
    sf::Text m_menuText;
    sf::Text m_lobbyText; 
    SimpleJsonParser m_jsonParser;

    void init();
    void update();
    void onEnd();
    void sDoAction(const Action & action);
    void sDoActionMouse(const Action & action, const Vec2 & pos);
    void sSend(std::string & message);

public:

    Scene_Lobby(GameEngine * gameEngine = nullptr);
    void sRender();
    void sReceive(std::string & message);
};