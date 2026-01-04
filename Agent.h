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

enum class AgentState
{
    Idle,
    Seek,
    Avoid
};

class Agent
{
public:
    Agent(Vector2 pos, Direction direction, float speed);  // konstruktor s počáteční pozicí
    void Update(Vector2 targetPos, World &world);       // logika agenta
	void Seek(World& world, Vector2 targetPos);
	void FSM(World& world, Vector2 targetPos); // konečný automat pro chování agenta
    void Draw() const;   // vykreslení agenta
    void Draw2() const;
	Vector2 GetPosition() const { return position; } // getter pro pozici
	float GetSpeed() const { return speed; } // getter pro rychlost
	float GetSize() const { return size; } // getter pro velikost

private:
    Vector2 position;    // pozice agenta
	Direction currentDirection; // aktuální směr pohybu
	Direction desiredDirection; // požadovaný směr pohybu
	Vector2 targetPosition; // cílová pozice
    float size;
    float speed;
    Direction lastDirection;
	AgentState currentState;
	float changeDirectionTimer = 0.0f;
};
