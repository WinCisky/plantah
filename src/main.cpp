#include <SFML/Graphics.hpp>

#include "GameEngine.h"
#include <iostream>

int main()
{
    std::cout << "Game Over" << std::endl;
    GameEngine ge = GameEngine("");
    ge.run();


    return 0;
}