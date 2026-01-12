#include "Agent.h"
#include "World.h"
#include <algorithm>

Agent::Agent(Vector2 pos, Direction direction, float speed) : position(pos), currentDirection(direction), 
targetPosition({ 0,0 }), speed(speed), size(20), lastDirection(direction), changeDirectionTimer(0.0f),
currentState(AgentState::Seek), desiredDirection(Direction::RIGHT), startAvoiding({ 0,0 }), first(false),
lastTarget({ -1,-1 }), pathIndex(0)
{
    path = {};
}

void Agent::Update(Vector2 targetPos, World &world)
{
	// Bug pokud je agent locknuty na target ktery je za zdi, tak se snazi furt dostat k nemu a neumi se vyhnout
	targetPosition = targetPos;
	//Seek(world, targetPosition);  // volání seek metody pro pohyb k cíli
	//FSM(world, targetPosition); // volání FSM pro chování agenta
	BFS(world, targetPosition);

}


void Agent::Draw() const
{
	DrawRectangleV(position, { size,size }, BLUE); //blue square
}

void Agent::Draw2() const
{
    DrawRectangleV(position, { size,size }, GREEN); //green square
}

void Agent::Seek(World& world, Vector2 targetPos)
{
    bool canMoveLeft = world.testLeft(*this);
    bool canMoveRight = world.testRight(*this);
    bool canMoveUp = world.testUp(*this);
    bool canMoveDown = world.testDown(*this);
    float directionx = targetPosition.x - position.x;
    float directiony = targetPosition.y - position.y;
    float length = sqrt(directionx * directionx + directiony * directiony);
    if (length > 0.0f)
    {
        directionx /= length;
        directiony /= length;

        // test pohybu X
        if (directionx > 0 && canMoveRight)
            position.x += directionx * speed;
        else if (directionx < 0 && canMoveLeft)
            position.x += directionx * speed;

        // test pohybu Y
        if (directiony > 0 && canMoveDown)
            position.y += directiony * speed;
        else if (directiony < 0 && canMoveUp)
            position.y += directiony * speed;
    }
}

void Agent::FSM(World& world, Vector2 targetPos)
{
    // rozdíl k cíli
    float dx = targetPos.x - position.x;
    float dy = targetPos.y - position.y;
    switch (currentState)
    {
        case AgentState::Idle:
            // nic neděláme
            break;

        case AgentState::Seek:
        {
            // zjistíme, kam můžeme jít
            bool canMoveLeft = world.testLeft(*this);
            bool canMoveRight = world.testRight(*this);
            bool canMoveUp = world.testUp(*this);
            bool canMoveDown = world.testDown(*this);
            bool moved = false;

            // pohyb po X ose
            if (dx > 0 && canMoveRight && lastDirection != Direction::LEFT) {
                position.x += speed;
                moved = true;
				lastDirection = Direction::RIGHT;
            }
            else if (dx < 0 && canMoveLeft && lastDirection != Direction::RIGHT) {
                position.x -= speed;
                moved = true;
				lastDirection = Direction::LEFT;
            }
            else if (dy < 0 && canMoveUp && lastDirection != Direction::DOWN) {
                position.y -= speed;
                moved = true;
				lastDirection = Direction::UP;
            }
            else if (dy > 0 && canMoveDown && lastDirection != Direction::UP) {
                position.y += speed;
                moved = true;
				lastDirection = Direction::DOWN;
            }
            if (!moved)
            {
                currentState = AgentState::Avoid;
                if(dx > 0 && !canMoveRight)
                    desiredDirection = Direction::RIGHT;
                else if (dx < 0 && !canMoveLeft)
                    desiredDirection = Direction::LEFT;
                else if (dy > 0 && !canMoveDown)
                    desiredDirection = Direction::DOWN;
                else if (dy < 0 && !canMoveUp)
					desiredDirection = Direction::UP;

            }
            break;
        }
        case AgentState::Avoid:
        {
            // zjistíme kam můžeme
            bool canLeft = world.testLeft(*this);
            bool canRight = world.testRight(*this);
            bool canUp = world.testUp(*this);
            bool canDown = world.testDown(*this);

            switch (desiredDirection)
            {
            case Direction::RIGHT:
            {
                if (canUp) {
                    position.y -= speed;
					lastDirection = Direction::UP;
                }
                else if (canLeft) {
                    position.x -= speed;
					lastDirection = Direction::LEFT;
                    desiredDirection = Direction::LEFT;
                }
                else if (canDown) {
                    position.y += speed;
					lastDirection = Direction::DOWN;
                    desiredDirection = Direction::DOWN;
                }
                break;
            }
            case Direction::LEFT:
            {
                if (canDown) {
                    position.y += speed;
                    lastDirection = Direction::DOWN;
                }
                else if (canRight) {
                    position.x += speed;
					lastDirection = Direction::RIGHT;
                    desiredDirection = Direction::RIGHT;
                }
                else if (canUp) {
                    position.y -= speed;
                    lastDirection = Direction::UP;
                    desiredDirection = Direction::UP;
                }
                break;
            }
            case Direction::UP:
            {
                if (canLeft) {
                    position.x -= speed;
                    lastDirection = Direction::LEFT;
                }
                else if (canDown) {
                    position.y += speed;
                    lastDirection = Direction::DOWN;
                    desiredDirection = Direction::DOWN;
                }
                else if (canRight) {
                    position.x += speed;
                    lastDirection = Direction::RIGHT;
                    desiredDirection = Direction::RIGHT;
                }
                break;
            }
            case Direction::DOWN:
            {
                if (canRight) {
                    position.x += speed;
                    lastDirection = Direction::RIGHT;
                }
                else if (canUp) {
                    position.y -= speed;
                    lastDirection = Direction::UP;
                    desiredDirection = Direction::UP;
                }
                else if (canLeft) {
                    position.x -= speed;
                    lastDirection = Direction::LEFT;
                    desiredDirection = Direction::LEFT;
                }
                break;
            }
            }
            if (desiredDirection == Direction::RIGHT && canRight && dx > 0)
                currentState = AgentState::Seek;
            else if (desiredDirection == Direction::LEFT && canLeft && dx < 0)
                currentState = AgentState::Seek;
            else if (desiredDirection == Direction::UP && canUp && dy < 0)
                currentState = AgentState::Seek;
            else if (desiredDirection == Direction::DOWN && canDown && dy > 0)
                currentState = AgentState::Seek;
            else if(position.x > 790 || position.x < 10 || position.y > 590 || position.y < 10)
				currentState = AgentState::Seek;
        }
    }
}

void Agent::BFS(World& world, Vector2 targetPos)
{
    bool needRepath = false;

    // 1️⃣ žádná cesta
    if (path.empty())
        needRepath = true;

    // 2️⃣ změna cíle
    if (targetPos.x != lastTarget.x || targetPos.y != lastTarget.y)
        needRepath = true;

    // 3️⃣ zkontrolovat, jestli další bod path není nyní zablokovaný
    if (!needRepath && pathIndex < path.size())
    {
        Vector2 next = path[pathIndex];
        gridPos nextGrid = world.ToGridPosition(next);
        if (!world.IsCellWalkable(nextGrid.x, nextGrid.y))
            needRepath = true;
    }

    // 4️⃣ enemy blízko → zkontroluj další krok
    if (!needRepath && !closeEnemies.empty() && pathIndex < path.size())
    {
        Vector2 next = path[pathIndex];
        for (const Vector2& enemyPos : closeEnemies)
        {
            float distToEnemy = sqrt((enemyPos.x - next.x) * (enemyPos.x - next.x) +
                (enemyPos.y - next.y) * (enemyPos.y - next.y));
            if (distToEnemy < 40.0f)
            {
                needRepath = true;
                break;
            }
        }
    }

    // === přepočítání cesty, pokud je potřeba ===
    if (needRepath)
    {
        // pokud je cílová pozice zablokovaná, najdi nejbližší volnou buňku
        gridPos goalG = world.ToGridPosition(targetPos);
        if (!world.IsCellWalkable(goalG.x, goalG.y))
        {
            // jednoduchý posun na nejbližší volnou buňku (1 krok ve čtyřech směrech)
            const int dx[4] = { 0, 0, -1, 1 };
            const int dy[4] = { -1, 1, 0, 0 };
            bool found = false;
            for (int i = 0; i < 4 && !found; ++i)
            {
                gridPos n = { goalG.x + dx[i], goalG.y + dy[i] };
                if (world.IsCellWalkable(n.x, n.y))
                {
                    targetPos = world.ToWorldPosition(n);
                    found = true;
                }
            }
            if (!found)
            {
                targetPos = position; // fallback - zůstaň na místě
            }
        }

        path = world.FindPathBFS(position, targetPos);
        pathIndex = 0;
        lastTarget = targetPos;
    }

    // === pohyb po cestě ===
    if (pathIndex < path.size())
    {
        Vector2 next = path[pathIndex];
        Vector2 moveDir = { next.x - position.x, next.y - position.y };
        float length = sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);

        if (length > 0.01f)
        {
            moveDir.x /= length;
            moveDir.y /= length;

            // ✅ lokální vyhnutí enemy
            for (const Vector2& enemyPos : closeEnemies)
            {
                Vector2 toEnemy = { enemyPos.x - position.x, enemyPos.y - position.y };
                float dist = sqrt(toEnemy.x * toEnemy.x + toEnemy.y * toEnemy.y);

                if (dist < 40.0f && dist > 0.01f)
                {
                    toEnemy.x /= dist;
                    toEnemy.y /= dist;
                    moveDir.x -= toEnemy.x;
                    moveDir.y -= toEnemy.y;

                    // normalizace
                    float len = sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
                    if (len > 0.01f)
                    {
                        moveDir.x /= len;
                        moveDir.y /= len;
                    }
                    break; // zpracujeme jen první blízký enemy
                }
            }

            position.x += moveDir.x * speed;
            position.y += moveDir.y * speed;
        }

        // pokud jsme dost blízko, jdeme na další bod
        float distanceToNext = sqrt((position.x - next.x) * (position.x - next.x) +
            (position.y - next.y) * (position.y - next.y));

        if (distanceToNext < speed * 0.9f) // trochu menší, aby nedošlo k zaseknutí
        {
            pathIndex++;
        }

        // nevymazávej closeEnemies příliš brzy!
        // closeEnemies.clear(); -> přesunout až po Update()
    }
}





bool Agent::isEnemyNear(const Vector2& enemyPos, float detectionRadius) const
{
    float dx = enemyPos.x - position.x;
    float dy = enemyPos.y - position.y;
    float distanceSquared = dx * dx + dy * dy;
    return distanceSquared <= detectionRadius * detectionRadius;
}

bool Agent::IsPositionSafe(const Vector2& pos) const
{
    const float dangerRadius = 60.0f; // musí být <= tomu co dáváš do isEnemyNear

    for (const auto& enemyPos : closeEnemies)
    {
        float dx = pos.x - enemyPos.x;
        float dy = pos.y - enemyPos.y;
        float dist = sqrt(dx * dx + dy * dy);

        if (dist < dangerRadius)
            return false;
    }
    return true;
}


EnemyAgent::EnemyAgent(Vector2 pos, float speed, Direction direction, float timer) : position(pos),
targetPosition(pos), speed(speed), currentDirection(direction), size(20), changeDirectionTimerDefault(timer), changeDirectionTimer(timer)
{

}

void EnemyAgent::Update(World& world)
{
    Agent temp(position, currentDirection, speed);
    if(changeDirectionTimer > 0.0f)
    {
        changeDirectionTimer -= 1.0f; // Předpokládáme, že Update je voláno 60x za sekundu
        if(currentDirection == Direction::UP)
        {
            if(world.testUp(temp) && position.y > 20)
                position.y -= speed;
            else
				changeDirectionTimer = 0.0f;
        }
        else if(currentDirection == Direction::DOWN)
        {
            if(world.testDown(temp) && position.y < 580)
                position.y += speed;
			else
				changeDirectionTimer = 0.0f;
        }
        else if(currentDirection == Direction::LEFT)
        {
            if(world.testLeft(temp) && position.x > 20)
                position.x -= speed;
            else
				changeDirectionTimer = 0.0f;
        }
        else if(currentDirection == Direction::RIGHT)
        {
            if(world.testRight(temp) && position.x < 780)
                position.x += speed;
            else
				changeDirectionTimer = 0.0f;
		}
    }
    else
    {
        if(currentDirection == Direction::UP)
			currentDirection = Direction::DOWN;
		else if (currentDirection == Direction::DOWN)
            currentDirection = Direction::UP;
        else if (currentDirection == Direction::LEFT)
            currentDirection = Direction::RIGHT;
        else if (currentDirection == Direction::RIGHT)
            currentDirection = Direction::LEFT;
		changeDirectionTimer = changeDirectionTimerDefault;
	}
}




void EnemyAgent::Draw() const
{
    DrawRectangleV(position, { size, size }, RED);
}

