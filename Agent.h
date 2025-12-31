#pragma once
#include "raylib.h"
#include "math.h"

class World; // forward declaration

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
    Agent(Vector2 pos, Direction direction, float speed);  // konstruktor s počáteční pozicí
    void Update(Vector2 targetPos, World &world);       // logika agenta
	void Update2(World &world);
    void Draw() const;   // vykreslení agenta
    void Draw2() const;
	Vector2 GetPosition() const { return position; } // getter pro pozici
	float GetSpeed() const { return speed; } // getter pro rychlost
	float GetSize() const { return size; } // getter pro velikost   

private:
    Vector2 position;    // pozice agenta
	Direction currentDirection; // aktuální směr pohybu
	Vector2 targetPosition; // cílová pozice
    float size;
    float speed;
    Direction lastDirection;
	float changeDirectionTimer = 0.0f;
};
