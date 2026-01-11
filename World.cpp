#include "World.h"
#include <queue>
#include <unordered_map>

World::World()
    : agent({ 100, 100 }, Direction::RIGHT, 4.0f),
    agent2({ 700, 500 }, Direction::LEFT, 5.0f)
{
    //enemies
    enemies.emplace_back(
        EnemyAgent({ 100, 100 }, 2.0f, Direction::RIGHT, 200.0f)
    );
    enemies.emplace_back(
        EnemyAgent({ 500, 200 }, 2.5f, Direction::UP, 200.0f)
    );
    enemies.emplace_back(
        EnemyAgent({ 200, 500 }, 3.0f, Direction::LEFT, 200.0f)
    );

    // horizontální zdi (x, y, width, height) -- vše násobek CELL_SIZE
    walls.emplace_back(Vector2{ 300, 200 }, Vector2{ 200, 20 }); // 10x1 buněk
    walls.emplace_back(Vector2{ 500, 340 }, Vector2{ 140, 20 }); // 7x1 buněk
    walls.emplace_back(Vector2{ 200, 100 }, Vector2{ 100, 20 }); // 5x1 buněk
    walls.emplace_back(Vector2{ 600, 440 }, Vector2{ 140, 20 }); // 7x1 buněk
    walls.emplace_back(Vector2{ 100, 240 }, Vector2{ 140, 20 }); // 7x1 buněk

    // vertikální zdi
    walls.emplace_back(Vector2{ 140, 400 }, Vector2{ 20, 140 }); // 1x7 buněk
    walls.emplace_back(Vector2{ 400, 100 }, Vector2{ 20, 200 }); // 1x10 buněk
    walls.emplace_back(Vector2{ 600, 240 }, Vector2{ 20, 140 }); // 1x7 buněk
    walls.emplace_back(Vector2{ 340, 300 }, Vector2{ 20, 100 }); // 1x5 buněk
    walls.emplace_back(Vector2{ 700, 140 }, Vector2{ 20, 200 }); // 1x10 buněk

    // bludišťové bloky / překážky
    walls.emplace_back(Vector2{ 240, 500 }, Vector2{ 100, 20 }); // 5x1 buněk
    walls.emplace_back(Vector2{ 440, 200 }, Vector2{ 20, 140 }); // 1x7 buněk
    walls.emplace_back(Vector2{ 540, 100 }, Vector2{ 140, 20 }); // 7x1 buněk
    walls.emplace_back(Vector2{ 140, 140 }, Vector2{ 20, 140 }); // 1x7 buněk

    // přidáme pár extra "bludišťových" zdí pro zajímavější pohyb
    walls.emplace_back(Vector2{ 300, 300 }, Vector2{ 60, 20 }); // 3x1 buněk
    walls.emplace_back(Vector2{ 500, 450 }, Vector2{ 20, 60 }); // 1x3 buněk
    walls.emplace_back(Vector2{ 200, 400 }, Vector2{ 60, 20 }); // 3x1 buněk
    walls.emplace_back(Vector2{ 650, 300 }, Vector2{ 20, 60 }); // 1x3 buněk
    walls.emplace_back(Vector2{ 400, 500 }, Vector2{ 60, 20 }); // 3x1 buněk
}

void World::Update()
{
	Vector2 targetPos = agent2.GetPosition(); // získání pozice druhého agenta
    agent.Update(targetPos, *this); // update agenta 
    for (auto& enemy : enemies)
    {
		enemy.Update(*this);
    }
    bool collision = false;
	collision = testCollisionAgents(agent, agent2);
    if (collision) {
        Vector2 startPos2 = GetRandomFreePosition();
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
    for(const EnemyAgent& enemy : enemies)
    {
        enemy.Draw();
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

Vector2 World::GetRandomFreePosition()
{
    std::vector<gridPos> freeCells;

    // projdeme všechny buňky podle velikosti GRID
    int maxX = 800 / CELL_SIZE;
    int maxY = 600 / CELL_SIZE;

    for (int x = 0; x < maxX; ++x)
    {
        for (int y = 0; y < maxY; ++y)
        {
            if (IsCellWalkable(x, y))
            {
                freeCells.push_back({ x, y });
            }
        }
    }

    if (freeCells.empty())
        return { CELL_SIZE, CELL_SIZE }; // fallback

    int index = GetRandomValue(0, static_cast<int>(freeCells.size() - 1));
    return ToWorldPosition(freeCells[index]);
}


gridPos World::ToGridPosition(Vector2 position) const
{
    return { static_cast<int>(position.x / CELL_SIZE), static_cast<int>(position.y / CELL_SIZE) };
}

Vector2 World::ToWorldPosition(gridPos gPos) const
{
    return { static_cast<float>(gPos.x * CELL_SIZE), static_cast<float>(gPos.y * CELL_SIZE) };
}

bool World::IsCellWalkable(int gridX, int gridY) const
{
    Rectangle cellRect = { gridX * CELL_SIZE, gridY * CELL_SIZE, (float)CELL_SIZE, (float)CELL_SIZE };

    for (const Wall& wall : walls)
    {
        Rectangle wallRect = { wall.GetPosition().x, wall.GetPosition().y,
                               wall.GetSize().x, wall.GetSize().y };
        if (CheckCollisionRecs(cellRect, wallRect))
            return false;
    }

    return true;
}

std::vector<Vector2> World::FindPathBFS(Vector2 start, Vector2 goal) const
{
    // Převod startu a cíle na gridové souřadnice
    gridPos startG = ToGridPosition(start);
    gridPos goalG = ToGridPosition(goal);

    // BFS fronta
    std::queue<gridPos> q;
    q.push(startG);

    // Mapování gridPos -> předchůdce, pro rekonstrukci cesty
    std::unordered_map<int, gridPos> cameFrom; // klíč = gridY*1000 + gridX
    auto key = [](const gridPos& g) { return g.y * 1000 + g.x; };
    cameFrom[key(startG)] = startG; // start nemá předchůdce

    bool found = false;

    while (!q.empty())
    {
        gridPos current = q.front();
        q.pop();

        if (current.x == goalG.x && current.y == goalG.y)
        {
            found = true;
            break;
        }

        // 4 sousední buňky (nahoru, dolu, doleva, doprava)
        const int dx[4] = { 0, 0, -1, 1 };
        const int dy[4] = { -1, 1, 0, 0 };

        for (int i = 0; i < 4; i++)
        {
            gridPos neighbor = { current.x + dx[i], current.y + dy[i] };

            // Kontrola průchodnosti a že jsme ještě nenavštívili
            if (IsCellWalkable(neighbor.x, neighbor.y) && cameFrom.find(key(neighbor)) == cameFrom.end())
            {
                cameFrom[key(neighbor)] = current;
                q.push(neighbor);
            }
        }
    }

    // Rekonstrukce cesty
    std::vector<Vector2> path;
    if (found)
    {
        gridPos current = goalG;
        while (!(current.x == startG.x && current.y == startG.y))
        {
            path.push_back(ToWorldPosition(current));
            current = cameFrom[key(current)];
        }
        path.push_back(ToWorldPosition(startG));

        // Obrátíme, aby cesta šla od startu ke goal
        std::reverse(path.begin(), path.end());
    }
    else
    {
        // Pokud cesta neexistuje, vrať start i goal jako fallback
        path.push_back(start);
        path.push_back(goal);
    }

    return path;
}
