#pragma once

#include "Scene.h"
#include <map>
#include <memory>
#include <deque>

#include "EntityManager.h"

class Scene_Lobby : public Scene
{

protected:

    std::string m_title;
    sf::Text m_menuText;

    void init();
    void update();
    void onEnd();
    void sDoAction(const Action & action);
    void sDoActionMouse(const Action & action, const Vec2 & pos);

public:

    Scene_Lobby(GameEngine * gameEngine = nullptr);
    void sRender();
};