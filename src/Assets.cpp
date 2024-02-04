#include "Assets.h"
#include "Animation.h"
#include <SFML/Graphics.hpp>

Assets::Assets() {
    // Qui potresti caricare i tuoi font e le tue animazioni
}

void Assets::addTexture(const std::string& name, const std::string& path)
{
	// TODO: implement this
}

void Assets::addAnimation(const std::string& name, const Animation& animation)
{
	// TODO: implement this
}

void Assets::addFont(const std::string& name, const std::string& path)
{
	// TODO: implement this
}


const sf::Texture& Assets::getTexture(const std::string& name) const
{
	return m_textures.at(name);
}

const sf::Font & Assets::getFont(const std::string & font) const {
    static sf::Font emptyFont;
    emptyFont.loadFromFile(font);
    return emptyFont;
}

const Animation & Assets::getAnimation(const std::string & name) const {
    // Qui dovresti restituire l'animazione richiesta dal tuo storage di animazioni
    // Per ora, restituirò solo un'animazione vuota
    static Animation emptyAnimation;
    return emptyAnimation;
}

void Assets::loadFromFile(const std::string& path)
{
	// TODO: implement this
}