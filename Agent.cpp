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
    float epsilon = 0.5f;
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


void Agent::Draw() const
{
	DrawRectangleV(position, { size,size }, BLUE); //blue square
}

void Agent::Draw2() const
{
    DrawRectangleV(position, { size,size }, RED); //red square
}
