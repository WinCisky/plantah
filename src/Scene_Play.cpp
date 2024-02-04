#include "Scene_Play.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"

#include <iostream>

Scene_Play::Scene_Play(GameEngine * gameEngine, const std::string & levelPath)
    : Scene(gameEngine)
    , m_levelPath(levelPath)
    , m_gridText(m_game->assets().getFont("Tech"), "", 12)
{
    init(m_levelPath);
}

void Scene_Play::init(const std::string & levelPath)
{
    registerAction(sf::Keyboard::Key::P, "PAUSE");
    registerAction(sf::Keyboard::Key::Escape, "QUIT");
    registerAction(sf::Keyboard::Key::T, "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::Key::C, "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::Key::G, "TOGGLE_GRID");

    // TODO: Register all other fameplay Actions

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Tech"));

    loadLevel(levelPath);
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
    // TODO: This function takes in a grid (x,y) position and an Entity
    // return a Vec2 indicating where the CENTER position of the Entity should be
    // you must use the Entity's Animation size to position it correctly
    // the size of the grid width and height is stored in m_gridSize.x and m_gridSize.y
    // the bottom-left corner of the Animation should align with the bottom left of the grid

    return Vec2(0,0);
}

void Scene_Play::loadLevel(const std::string & filename)
{
    // reset the entity manager every time we load a level
    m_entityManager = EntityManager();

    // TODO: read in the level file and add the appropriate entities
    // use the PlayerConfig struct m_playerConfig to store player proprieties
    // this struct is defined at the top of Scene_Play.h

    // NOTE: all of the code below is sample code which shows you how to
    // set up and use entities with the new syntax, it should be removed

    spawnPlayer();

    // some sample entities
    auto brick = m_entityManager.addEntity("tile");
    // IMPORTANT: always add the CAnimation component first so that gridToMidPixel con compute correctly
    brick->addComponent<CAnimation>(m_game->assets().getAnimation("Brick"), true);
    brick->addComponent<CTransform>(Vec2(224, 480));
    //  NOTE: Your final code should position the entity with the grid x,y position read from the file:
    // brick->addComponentzCTransform>(gridToMidPixel(gridX, gridY, brick));

    if (brick->getComponent<CAnimation>().animation.getName() == "Brick")
    {
        std::cout << "This could be a good way of identifying if a tile is a brick!" << std::endl;
    }

    auto block = m_entityManager.addEntity("tile");
    block->addComponent<CAnimation>(m_game->assets().getAnimation("Block"), true);
    brick->addComponent<CTransform>(Vec2(224, 480));
    // add a bounding box, this will now show up if we press the 'C' key
    block->addComponent<CBoundingBox>(m_game->assets().getAnimation("Block").getSize());

    auto question = m_entityManager.addEntity("tile");
    question->addComponent<CAnimation>(m_game->assets().getAnimation("Question"), true);
    question->addComponent<CTransform>(Vec2(352, 480));

    // NOTE: THIS IS INCREDIBLY IMPORTANT PLEASE READ THIS EXAMPLE
    // Components are now returned as references rather than pointers
    // if you do not specify a reference variabletype, it will COPY the component
    // here is an example:
    //
    // this will COPY the transform into the variable 'transform1' - it is INCORRECT
    // any changes you make to transform1 will not be changed inside the entity
    // auto transform1 = entity->get<CTransform>();
    //
    // this will REFERENCE the transform with the variable 'transform2' - it is CORRECT
    // now any changes you make to transform2 will be changed inside the entity
    // auto & transform2 = entity->get<CTransform>();
}

void Scene_Play::spawnPlayer()
{
    // here is a sample player entity which you can use to construct other entities
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Stand"), true);
    m_player->addComponent<CTransform>(Vec2(224, 352));
    m_player->addComponent<CBoundingBox>(Vec2(48, 48));

    // TODO: be sure to add the remaining components to the player
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
    // TODO: this should spawn a bullet at the given entity, going in the direction the entity is facing
}

void Scene_Play::update()
{
    m_entityManager.update();

    // TODO: implement pause functionality

    sMovement();
    sLifespan();
    sCollision();
    sAnimation();
    sRender();
}

void Scene_Play::sMovement()
{
    // TODO: implement player movement / jumping based on its CInput component
    // TODO: implement gravity's effect on the player
    // TODO: Implement the maximum player speed in both X and Y directions
    // NOTE: Setting an entity's scale.x to -1/1 will make it face to the left/right
}

void Scene_Play::sLifespan()
{
    // TODO: check lifespan of entities that have them, and destroy them if they go over
}

void Scene_Play::sCollision()
{
    // REMEMBER: SFML's (0,0) position is on the TOP-LEFT corner
    // this means jumping will have a negative y-component
    // and gravity will have a positive y-component
    // also, something BELOW something else will have a y value GREATER than it
    // also, something ABOVE something else will have a y value LESS than it

    // TODO: implement Physics::GetOverlap() function, use it inside this function

    // TODO: implement bullet / tile collisions
    // destroy the tile if it has a Brick animation
    // TODO: implement player / tile collsions and resolutions
    // update the CState component of the player to store whether
    // it is currently on the ground or in the air. This will be
    // used by the Animation system
    // TODO: check to see if the player has fallen down a hole (y > height())
    // TODO: don't let the player walk off the left side of the map
}

void Scene_Play::sDoAction(const Action & action)
{
    if (action.type() == "START")
    {
        if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
        else if (action.name() == "TOGGLE_COLLISION") { m_drawCollisions = !m_drawCollisions; }
        else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
        else if (action.name() == "PAUSE") { setPaused(!m_paused); }
        else if (action.name() == "QUIT") { onEnd(); }
    }
    else if (action.type() == "END")
    {

    }
}

void Scene_Play::sAnimation()
{
    // TODO: complete the Animation class code first

    // TODO: set the animation of the player based on its CState component
    // TODO: for each entity with an animation, call entity->getComponent<CAnimation>().animation.update()
    // if the animation is not repeated, and it has ended, destroy the entity
}

void Scene_Play::onEnd()
{
    // TODO: when scene ends, chnage back to the MENU scene
    // use m_game->changeScene(correct params);
}

void Scene_Play::sRender()
{
    // color the background darker so you know that the game is paused
    if (!m_paused) { m_game->window().clear(sf::Color(100, 100, 255)); }
    else { m_game->window().clear(sf::Color(50, 50, 150)); }

    // set the viewport of the window to be centered on the player if it's far enough right
    auto & pPos = m_player->getComponent<CTransform>().pos;
    float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
    sf::View view = m_game->window().getView();
    view.setCenter(sf::Vector2f(windowCenterX, m_game->window().getSize().y - view.getCenter().y));
    m_game->window().setView(view);

    // draw all Entityt textures / animations
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities())
        {
            auto & transform = e->getComponent<CTransform>();

            if (e->hasComponent<CAnimation>())
            {
                auto & animation = e->getComponent<CAnimation>().animation;
                CTransform test;
                animation.getSprite().setRotation(sf::degrees(transform.angle));
                animation.getSprite().setPosition(sf::Vector2f(transform.pos.x, transform.pos.y));
                animation.getSprite().setScale(sf::Vector2f(transform.scale.x, transform.scale.y));
                m_game->window().draw(animation.getSprite());
            }
        }
    }

    // draw all Entity collisions bounding boxes with a rectangleshape
    if (m_drawCollisions)
    {
        for (auto e : m_entityManager.getEntities())
        {
            if (e->hasComponent<CBoundingBox>())
            {
                auto & box = e->getComponent<CBoundingBox>();
                auto & transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x-1, box.size.y-1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(sf::Vector2f(transform.pos.x, transform.pos.y));
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color(255, 255, 255, 255));
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);
            }
        }
    }

    // draw the grid so that students can easily debug
    if (m_drawGrid)
    {
        float leftX = m_game->window().getView().getCenter().x - width() / 2;
        float rightX = leftX + width() + m_gridSize.x;
        float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

        for (float x = nextGridX; x < rightX; x += m_gridSize.x)
        {
            drawLine(Vec2(x, 0), Vec2(x, height()));
        }

        for (float y = 0; y < height(); y += m_gridSize.y)
        {
            drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));
            for (float x = nextGridX; x < rightX; x += m_gridSize.x)
            {
                std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
                std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(sf::Vector2f(x + 3, height() - y - m_gridSize.y + 2));
                m_game->window().draw(m_gridText);
            }
        }        
    }
}