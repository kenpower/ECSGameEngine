#pragma once
#include"./ECSGameEngine/ECS.h"

void movementSystem(Components&, double);
void userControlSystem(Components&, double);
void collisionSystem(Components&);
void bounceSystem(Components&);
void deadBlocksSystem(Components&);
void scoreBlocksSystem(Components&, int&);

