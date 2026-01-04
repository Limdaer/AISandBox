#include "World.h"

World::World()
    : agent({ 100, 100}, Direction::RIGHT, 4.0f),agent2({700,500}, Direction::LEFT, 5.0f) // počáteční pozice agenta
{
    // horizontální zdi
    walls.emplace_back(Vector2{ 300, 200 }, Vector2{ 200, 20 });
    walls.emplace_back(Vector2{ 500, 350 }, Vector2{ 150, 20 });
    walls.emplace_back(Vector2{ 200, 100 }, Vector2{ 100, 20 });
    walls.emplace_back(Vector2{ 600, 450 }, Vector2{ 150, 20 });
    walls.emplace_back(Vector2{ 100, 250 }, Vector2{ 150, 20 });

    // vertikální zdi
    walls.emplace_back(Vector2{ 150, 400 }, Vector2{ 20, 150 });
    walls.emplace_back(Vector2{ 400, 100 }, Vector2{ 20, 200 });
    walls.emplace_back(Vector2{ 600, 250 }, Vector2{ 20, 150 });
    walls.emplace_back(Vector2{ 350, 300 }, Vector2{ 20, 100 });
    walls.emplace_back(Vector2{ 700, 150 }, Vector2{ 20, 200 });

    // přidáme pár "bludišťových" bloků pro komplikovanější obcházení
    walls.emplace_back(Vector2{ 250, 500 }, Vector2{ 100, 20 });
    walls.emplace_back(Vector2{ 450, 200 }, Vector2{ 20, 150 });
    walls.emplace_back(Vector2{ 550, 100 }, Vector2{ 150, 20 });
    walls.emplace_back(Vector2{ 150, 150 }, Vector2{ 20, 150 });
}

void World::Update()
{
	Vector2 targetPos = agent2.GetPosition(); // získání pozice druhého agenta
    agent.Update(targetPos, *this); // update agenta 
    bool collision = false;
	collision = testCollisionAgents(agent, agent2);
    if (collision) {
        Vector2 startPos2 = GetRandomFreePosition(agent2.GetSize());
        agent2 = Agent(startPos2, Direction::LEFT, 5.0f);
    }
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

bool World::testCollisionAgents(const Agent& agent1, const Agent& agent2) const
{
    Rectangle agent1Rect = { agent1.GetPosition().x, agent1.GetPosition().y, agent1.GetSize(), agent1.GetSize()};
    Rectangle agent2Rect = { agent2.GetPosition().x, agent2.GetPosition().y, agent2.GetSize(), agent2.GetSize()};
    return CheckCollisionRecs(agent1Rect, agent2Rect);
}

Wall::Wall(Vector2 pos, Vector2 size) : position(pos), size(size)
{
}

void Wall::Draw() const
{
    DrawRectangleV(position, size, GRAY); // vykreslení zdi
}

Vector2 World::GetRandomFreePosition(float agentSize)
{
    Vector2 pos;
    bool colliding;

    const int maxAttempts = 100; // aby se funkce nezasekla v přeplněném světě
    int attempts = 0;

    do
    {
        pos.x = GetRandomValue((int)agentSize, 800 - (int)agentSize);
        pos.y = GetRandomValue((int)agentSize, 600 - (int)agentSize);

        colliding = false;
        Rectangle agentRect = { pos.x, pos.y, agentSize, agentSize };

        for (const Wall& wall : walls)
        {
            Rectangle wallRect = { wall.GetPosition().x, wall.GetPosition().y,
                                   wall.GetSize().x, wall.GetSize().y };
            if (CheckCollisionRecs(agentRect, wallRect))
            {
                colliding = true;
                break;
            }
        }

        attempts++;
    } while (colliding && attempts < maxAttempts);

    // pokud by se nepodařilo najít místo, vrátíme defaultní pozici
    if (colliding) pos = Vector2{ agentSize, agentSize };

    return pos;
}
