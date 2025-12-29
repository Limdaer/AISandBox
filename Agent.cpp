#include "Agent.h"

Agent::Agent(Vector2 pos, Direction direction) : position(pos), currentDirection(direction) {}

void Agent::Update()
{
	float speed = 5.0f;

    switch (currentDirection)
    {
    case Direction::RIGHT:
        position.x += speed;
        if (position.x >= 700) currentDirection = Direction::DOWN;
        break;
    case Direction::DOWN:
        position.y += speed;
        if (position.y >= 500) currentDirection = Direction::LEFT;
        break;
    case Direction::LEFT:
        position.x -= speed;
        if (position.x <= 100) currentDirection = Direction::UP;
        break;
    case Direction::UP:
        position.y -= speed;
        if (position.y <= 100) currentDirection = Direction::RIGHT;
        break;
    }

}

void Agent::Draw() const
{
	DrawRectangleV(position, { 20, 20 }, BLUE); //blue square
}

void Agent::Draw2() const
{
    DrawRectangleV(position, { 20, 20 }, RED); //blue square
}
