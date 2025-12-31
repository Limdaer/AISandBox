#include "World.h"

World::World()
    : agent({ 100, 100}, Direction::RIGHT, 4.0f),agent2({700,500}, Direction::LEFT, 5.0f) // počáteční pozice agenta
{
	walls.emplace_back(Vector2{ 300, 200 }, Vector2{ 200, 20 }); // přidání zdi do světa
	walls.emplace_back(Vector2{ 150, 400 }, Vector2{ 20, 150 }); // přidání další zdi do světa
	walls.emplace_back(Vector2{ 500, 350 }, Vector2{ 150, 20 }); // přidání další zdi do světa
	walls.emplace_back(Vector2{ 400, 100 }, Vector2{ 20, 200 }); // přidání další zdi do světa
	walls.emplace_back(Vector2{ 600, 250 }, Vector2{ 20, 150 }); // přidání další zdi do světa
}

void World::Update()
{
	Vector2 targetPos = agent2.GetPosition(); // získání pozice druhého agenta
    agent.Update(targetPos, *this); // update agenta
    agent2.Update2(*this);   
}

void World::Draw()
{
    agent.Draw();   // vykreslení agenta
	agent2.Draw2(); // vykreslení druhého agenta
    for (const Wall& wall : walls)
    {
        wall.Draw();
    }
}

bool World::testRight(const Agent& agent) const
{
	Rectangle agentRect = { agent.GetPosition().x + agent.GetSpeed(), agent.GetPosition().y, agent.GetSize(), agent.GetSize()};
    for (int i = 0; i < walls.size(); i++)
    {
		Rectangle WallRect = { walls[i].GetPosition().x, walls[i].GetPosition().y, walls[i].GetSize().x, walls[i].GetSize().y };
        if (CheckCollisionRecs(agentRect, WallRect))
        {
            return false;
        }
	}
    return true;
}

bool World::testLeft(const Agent& agent) const
{
    Rectangle agentRect = { agent.GetPosition().x - agent.GetSpeed(), agent.GetPosition().y, agent.GetSize(), agent.GetSize()};
    for (int i = 0; i < walls.size(); i++)
    {
        Rectangle WallRect = { walls[i].GetPosition().x, walls[i].GetPosition().y, walls[i].GetSize().x, walls[i].GetSize().y };
        if (CheckCollisionRecs(agentRect, WallRect))
        {
            return false;
        }
    }
    return true;
}

bool World::testUp(const Agent& agent) const
{
    Rectangle agentRect = { agent.GetPosition().x, agent.GetPosition().y - agent.GetSpeed(), agent.GetSize(), agent.GetSize()};
    for (int i = 0; i < walls.size(); i++)
    {
        Rectangle WallRect = { walls[i].GetPosition().x, walls[i].GetPosition().y, walls[i].GetSize().x, walls[i].GetSize().y };
        if (CheckCollisionRecs(agentRect, WallRect))
        {
            return false;
        }
    }
    return true;
}

bool World::testDown(const Agent& agent) const
{
    Rectangle agentRect = { agent.GetPosition().x, agent.GetPosition().y + agent.GetSpeed(), agent.GetSize(), agent.GetSize()};
    for (int i = 0; i < walls.size(); i++)
    {
        Rectangle WallRect = { walls[i].GetPosition().x, walls[i].GetPosition().y, walls[i].GetSize().x, walls[i].GetSize().y };
        if (CheckCollisionRecs(agentRect, WallRect))
        {
            return false;
        }
    }
    return true;
}

Wall::Wall(Vector2 pos, Vector2 size) : position(pos), size(size)
{
}

void Wall::Draw() const
{
    DrawRectangleV(position, size, GRAY); // vykreslení zdi
}
