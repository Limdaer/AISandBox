#include "Agent.h"

Agent::Agent(Vector2 pos) : position(pos) {}

void Agent::Update()
{
    // zatím žádná logika – agent stojí
}

void Agent::Draw() const
{
    DrawRectangleV(position, { 20, 20 }, BLUE); // vykreslí modrý čtverec 20x20 px
}
