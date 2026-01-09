#include "Agent.h"
#include "World.h"
#include <algorithm>

Agent::Agent(Vector2 pos, Direction direction, float speed) : position(pos), currentDirection(direction), 
targetPosition({ 0,0 }), speed(speed), size(20), lastDirection(direction), changeDirectionTimer(0.0f),
currentState(AgentState::Seek), desiredDirection(Direction::RIGHT), startAvoiding({ 0,0 }), first(false)
{}

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
    DrawRectangleV(position, { size,size }, RED); //red square
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

void Agent::BFS(World& world, Vector2 targetPos) {
    // Pokud se target změnil nebo cesta neexistuje, spočítáme novou
    if (path.empty() || targetPos.x != lastTarget.x || targetPos.y != lastTarget.y)
    {
        path = world.FindPathBFS(position, targetPos); // BFS vrací cestu po světových souřadnicích
        pathIndex = 0;
        lastTarget = targetPos;
    }

    // Pohyb po cestě
    if (pathIndex < path.size())
    {
        Vector2 next = path[pathIndex];
        Vector2 dir = { next.x - position.x, next.y - position.y };
        float length = sqrt(dir.x * dir.x + dir.y * dir.y);
        if (length > 0)
        {
            dir.x /= length;
            dir.y /= length;

            position.x += dir.x * speed;
            position.y += dir.y * speed;
        }

        // Pokud jsme dost blízko, jdeme na další bod
        if (fabs(position.x - next.x) < 1.0f && fabs(position.y - next.y) < 1.0f)
            pathIndex++;
    }
}

