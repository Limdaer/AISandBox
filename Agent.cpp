#include "Agent.h"
#include "World.h"
#include <algorithm>

Agent::Agent(Vector2 pos, Direction direction, float speed) : position(pos), currentDirection(direction), 
targetPosition({ 0,0 }), speed(speed), size(20), lastDirection(direction), changeDirectionTimer(0.0f),
currentState(AgentState::Seek), desiredDirection(Direction::RIGHT)
{}

void Agent::Update(Vector2 targetPos, World &world)
{
	// Bug pokud je agent locknuty na target ktery je za zdi, tak se snazi furt dostat k nemu a neumi se vyhnout
	targetPosition = targetPos;
	//Seek(world, targetPosition);  // volání seek metody pro pohyb k cíli
	FSM(world, targetPosition); // volání FSM pro chování agenta

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

            // rozdíl k cíli
            float dx = targetPos.x - position.x;
            float dy = targetPos.y - position.y;

            // pohyb po X ose
            if (dx > 0 && canMoveRight) {
                position.x += speed;
                moved = true;
				lastDirection = Direction::RIGHT;
            }
            else if (dy > 0 && canMoveDown) {
                position.y += speed;
                moved = true;
				lastDirection = Direction::DOWN;
            }
            else if (dx < 0 && canMoveLeft) {
                position.x -= speed;
                moved = true;
				lastDirection = Direction::LEFT;
            }
            else if (dy < 0 && canMoveUp) {
                position.y -= speed;
                moved = true;
				lastDirection = Direction::UP;
            }
            if (moved == false)
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
                if (canUp)
                    position.y -= speed;
                else if (canLeft)
                    position.x -= speed;
                else if (canDown)
                    position.y += speed;
                break;
            }
            case Direction::LEFT:
            {
                if (canDown)
                    position.y += speed;
                else if (canRight)
                    position.x += speed;
                else if (canUp)
                    position.y -= speed;
                break;
            }
            case Direction::UP:
            {
                if (canLeft)
                    position.x -= speed;
                else if (canDown)
                    position.y += speed;
                else if (canRight)
                    position.x += speed;
                break;
            }
            case Direction::DOWN:
            {
                if (canRight)
                    position.x += speed;
                else if (canUp)
                    position.y -= speed;
                else if (canLeft)
                    position.x -= speed;
                break;
            }
            }
            if(desiredDirection == Direction::RIGHT && canRight)
                currentState = AgentState::Seek;
            else if(desiredDirection == Direction::LEFT && canLeft)
                currentState = AgentState::Seek;
            else if(desiredDirection == Direction::UP && canUp)
                currentState = AgentState::Seek;
            else if(desiredDirection == Direction::DOWN && canDown)
				currentState = AgentState::Seek;
        }
    }
}