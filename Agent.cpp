#include "Agent.h"
#include "World.h"
#include <algorithm>

Agent::Agent(Vector2 pos, Direction direction, float speed) : position(pos), currentDirection(direction), 
targetPosition({ 0,0 }), speed(speed), size(20), lastDirection(direction), changeDirectionTimer(0.0f)
{}

void Agent::Update(Vector2 targetPos, World &world)
{
	// Bug pokud je agent locknuty na target ktery je za zdi, tak se snazi furt dostat k nemu a neumi se vyhnout
	targetPosition = targetPos;
	bool canMoveLeft = world.testLeft(*this);
	bool canMoveRight = world.testRight(*this);
	bool canMoveUp = world.testUp(*this);
	bool canMoveDown = world.testDown(*this);

    if(targetPosition.x >= position.x && canMoveRight)
    {
        position.x += speed;
    }
    else if(targetPosition.x <= position.x && canMoveLeft)
    {
        position.x -= speed;
	}
    if(targetPosition.y >= position.y && canMoveDown)
    {
        position.y += speed;
    }
    else if (targetPosition.y <= position.y && canMoveUp)
    {
        position.y -= speed;
    }
}

void Agent::Update2(World& world)
{


    std::vector<Direction> possibleDirections;
    if (world.testRight(*this) && position.x + speed < 700) possibleDirections.push_back(Direction::RIGHT);
    if (world.testLeft(*this) && position.x - speed > 100) possibleDirections.push_back(Direction::LEFT);
    if (world.testUp(*this) && position.y - speed > 100) possibleDirections.push_back(Direction::UP);
    if (world.testDown(*this) && position.y + speed < 500) possibleDirections.push_back(Direction::DOWN);

    bool lastIsPossible = std::find(possibleDirections.begin(), possibleDirections.end(), lastDirection) != possibleDirections.end();
    if (!possibleDirections.empty() && changeDirectionTimer <= 0.0f || !lastIsPossible )
    {
        int idx = GetRandomValue(0, possibleDirections.size() - 1);
        currentDirection = possibleDirections[idx];
    }
    else
		currentDirection = lastDirection;
    switch (currentDirection)
    {
    case Direction::RIGHT:
		lastDirection = Direction::RIGHT;
		changeDirectionTimer = 10.0f;
        position.x += speed;
        break;
    case Direction::LEFT:
		lastDirection = Direction::LEFT;
		changeDirectionTimer = 10.0f;
        position.x -= speed;
        break;
    case Direction::UP:
		lastDirection = Direction::UP;
		changeDirectionTimer = 10.0f;
        position.y -= speed;
        break;
    case Direction::DOWN:
		lastDirection = Direction::DOWN;
		changeDirectionTimer = 10.0f;
        position.y += speed;
        break;
    }
	changeDirectionTimer -= 1.0f;
}


void Agent::Draw() const
{
	DrawRectangleV(position, { size,size }, BLUE); //blue square
}

void Agent::Draw2() const
{
    DrawRectangleV(position, { size,size }, RED); //blue square
}
