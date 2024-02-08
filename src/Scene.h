#pragma once

#include "Action.h"
#include "EntityManager.h"

#include <memory>

class GameEngine;

typedef std::map<sf::Keyboard::Key, std::string> ActionMap;
typedef std::map<sf::Mouse::Button, std::string> ActionMapMouse;

class Scene
{

protected:

    GameEngine * m_game = nullptr;
    EntityManager m_entityManager;
    ActionMap m_actionMap;
    ActionMapMouse m_actionMapMouse;
    bool m_paused = false;
    bool m_hasEnded = false;
    size_t m_currentFrame = 0;

    virtual void onEnd() = 0;
    virtual void sSend(std::string & message) = 0;
    void setPaused(bool paused);

public:

    Scene();
    Scene(GameEngine * gameEngine);

    virtual void update() = 0;
    virtual void sDoAction(const Action & action) = 0;
    virtual void sDoActionMouse(const Action & action, const Vec2 & pos) = 0;
    virtual void sRender() = 0;
    virtual void sReceive(std::string & message) = 0;

    virtual void doAction(const Action & action);
    virtual void doActionMouse(const Action & action, const Vec2 & pos);
    void simulate(const size_t frames);
    void registerAction(sf::Keyboard::Key inputkey, const std::string & actionName);
    void registerActionMouse(sf::Mouse::Button input, const std::string & actionName);

    size_t width() const;
    size_t height() const;
    size_t currentFrame() const;

    bool hasEnded() const;
    const ActionMap & getActionMap() const;
    const ActionMapMouse & getActionMapMouse() const;
    void drawLine(const Vec2 & p1, const Vec2 & p2);
};