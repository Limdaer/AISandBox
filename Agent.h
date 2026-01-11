#pragma once
#include "raylib.h"
#include "math.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

class World; // forward declaration

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
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
    void BFS(World& world, Vector2 targetPos);
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
    Vector2 startAvoiding;
    bool first;
    std::vector<Vector2> path; 
    size_t pathIndex = 0;     
    Vector2 lastTarget;   

};

class EnemyAgent {
public:
    EnemyAgent(Vector2 pos, float speed, Direction direction, float timer);  // konstruktor s počáteční pozicí
    void Update(World &world);       // logika agenta
    void Draw() const;   // vykreslení agenta
    Vector2 GetPosition() const { return position; } // getter pro pozici
    float GetSpeed() const { return speed; } // getter pro rychlost
	float GetSize() const { return size; } // getter pro velikost

private:
	Vector2 position;    // pozice agenta
	Direction currentDirection; // aktuální směr pohybu
    Vector2 targetPosition; // cílová pozice
    float size;
	float speed;
	float changeDirectionTimer = 0.0f;
    float changeDirectionTimerDefault;
};
