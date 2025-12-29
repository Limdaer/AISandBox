// AISandBox.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "raylib.h"
#include "Game.h"
#include "World.h"
#include "Agent.h"

int main()
{
    Game game;   // Vytvoříme instanci hry
    game.Run();  // Spustíme hlavní loop hry
    return 0;
}
