#include "World.h"

World::World()
    : agent({ 100, 100 }) // po?áte?ní pozice agenta
{
}

void World::Update()
{
    agent.Update(); // update agenta
}

void World::Draw()
{
    agent.Draw();   // vykreslení agenta
}
