#pragma once
#include "Agent.h"
#include <vector>

const int CELL_SIZE = 20;

struct gridPos{
	int x;
	int y;
};

class Wall
{
public:
    Wall(Vector2 pos, Vector2 size); // konstruktor s pozicí a velikostí
	void Draw() const;               // vykreslení zdi
	Vector2 GetPosition() const { return position; } // getter pro pozici
	Vector2 GetSize() const { return size; }         // getter pro velikost
private:
    Vector2 position; // pozice zdi
	Vector2 size;     // velikost zdi
};

class World
{
public:
    World();         // konstruktor
    void Update();   // aktualizace sv?ta
    void Draw();     // vykreslení sv?ta
	bool testRight(const Agent& agent) const;
	bool testLeft(const Agent& agent) const;
	bool testUp(const Agent& agent) const;
	bool testDown(const Agent& agent) const;
	bool testCollisionAgents(const Agent& agent1, const Agent& agent2) const;
	bool IsCellWalkable(int gridX, int gridY) const;
	Vector2 ToWorldPosition(gridPos gPos) const;
	Vector2 GetRandomFreePosition();
	gridPos ToGridPosition(Vector2 position) const;
	std::vector<Vector2> FindPathBFS(Vector2 start, Vector2 goal) const;


private:
    Agent agent;     // prozatím jeden agent
	Agent agent2;    // druhý agent
    std::vector<Wall> walls;
	std::vector<EnemyAgent> enemies;
};
