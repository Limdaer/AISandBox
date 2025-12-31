#include "Game.h"
#include "World.h" 
#include "raylib.h"

void Game::Run()
{
    InitWindow(800, 600, "AI Sandbox"); // otevře okno 800x600
    SetTargetFPS(60);                  // nastavíme framerate

    World world;                        // vytvoříme instanci světa

    while (!WindowShouldClose())        // hlavní loop
    {
        world.Update();                 // aktualizace logiky světa

        BeginDrawing();                 // začátek kreslení
        ClearBackground(RAYWHITE);      // vyčistíme obrazovku
        world.Draw();                   // vykreslíme všechny objekty
        EndDrawing();                   // konec kreslení
    }

    CloseWindow();                      // zavře okno
}
