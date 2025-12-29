#pragma once
#include "Agent.h"

class World
{
public:
    World();         // konstruktor
    void Update();   // aktualizace sv?ta
    void Draw();     // vykreslení sv?ta

private:
    Agent agent;     // prozatím jeden agent
	Agent agent2;    // druhý agent
};
