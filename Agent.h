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
    Agent(Vector2 pos);  // konstruktor s počáteční pozicí
    void Update();       // logika agenta (zatím prázdná)
    void Draw() const;   // vykreslení agenta

private:
    Vector2 position;    // pozice agenta
	Direction currentDirection; // aktuální směr pohybu
};
