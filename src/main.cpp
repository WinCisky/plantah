#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

int main()
{
    auto videoMode = sf::VideoMode(sf::Vector2u(360, 640));
    sf::VideoMode screen(videoMode);
    sf::RenderWindow window(screen, "");
    window.setFramerateLimit(30);

    while (window.isOpen())
    {
        for (sf::Event event; window.pollEvent(event);)
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }

        window.clear();
        window.display();
    }
}