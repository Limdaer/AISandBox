#include "World.h"

World::World()
    : agent({ 100, 100 }, Direction::RIGHT),agent2({100,500}, Direction::UP) // počáteční pozice agenta
{
}

void World::Update()
{
    agent.Update(); // update agenta
    agent2.Update();   
}

void World::Draw()
{
    agent.Draw();   // vykreslení agenta
	agent2.Draw2(); // vykreslení druhého agenta
}
