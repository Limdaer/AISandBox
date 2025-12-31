#pragma once
#include "Agent.h"
#include <vector>

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

private:
    Agent agent;     // prozatím jeden agent
	Agent agent2;    // druhý agent
    std::vector<Wall> walls;
};
