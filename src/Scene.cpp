#include "Scene.h"

Scene::Scene()
{
}

Scene::Scene(GameEngine* gameEngine)
{
	m_game = gameEngine;
}

void Scene::doAction(const Action& action)
{
	sDoAction(action);
}

void Scene::doActionMouse(const Action& action, const Vec2 & pos)
{
	sDoActionMouse(action, pos);
}

void Scene::simulate(const size_t frames)
{
}


size_t Scene::width() const
{
	return 0;
}

size_t Scene::height() const
{
	return 0;
}

size_t Scene::currentFrame() const
{
	return 0;
}

bool Scene::hasEnded() const
{
	return false;
}


void Scene::drawLine(const Vec2& p1, const Vec2& p2)
{

}

void Scene::setPaused(bool paused)
{

}


/* handle actions */


void Scene::registerAction(sf::Keyboard::Key inputKey, const std::string& actionName)
{
	m_actionMap[inputKey] = actionName;
}

void Scene::registerActionMouse(sf::Mouse::Button input, const std::string& actionName)
{
	m_actionMapMouse[input] = actionName;
}

const ActionMap & Scene::getActionMap() const 
{
    return m_actionMap;
}

const ActionMapMouse & Scene::getActionMapMouse() const 
{
    return m_actionMapMouse;
}