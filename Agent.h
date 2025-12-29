#pragma once
#include "raylib.h"

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Agent
{
public:
    Agent(Vector2 pos, Direction direction);  // konstruktor s počáteční pozicí
    void Update();       // logika agenta
    void Draw() const;   // vykreslení agenta
    void Draw2() const;

private:
    Vector2 position;    // pozice agenta
	Direction currentDirection; // aktuální směr pohybu
};
