#pragma once

#include "Scene.h"
#include <map>
#include <memory>

#include "EntityManager.h"
#include "SimpleJsonParser.h"

class Scene_Play : public Scene
{
    struct PlayerConfig
    {
        float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
        std::string WEAPON;
    };
    
    int m_buttonSelected;
    std::map<std::string, sf::RectangleShape> m_buttons;

    void placeButtons();
    
public:
    Scene_Play(GameEngine * gameEngine);
    void sRender();
    void sReceive(std::string & message);

protected:

    SimpleJsonParser m_jsonParser;
    
    void init();
    void update();
    void onEnd();
    void sDoAction(const Action & action);
    void sDoActionMouse(const Action & action, const Vec2 & pos);
    void sSend(std::string & message);
};