#pragma once
#include"./ECSGameEngine/ECS.h"
#include "./ECSGameEngine/ConsoleRenderWindow.h"

void movementSystem(Components&, double);
void userControlSystem(Components&, double);
void collisionSystem(Components&);
void bounceSystem(Components&);
void deadBlocksSystem(Components&);

void scoreBlocksSystem(Components&, int&);

void renderCharOnConsoleSystem(Components&, ConsoleRenderWindow&); 
void renderStringOnConsoleSystem(Components&, ConsoleRenderWindow&);