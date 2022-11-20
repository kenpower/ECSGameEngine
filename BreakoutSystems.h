#pragma once
#include"ECS.h"
#include "ConsoleRenderWindow.h"

void movementSystem(Entities&, double);
void userControlSystem(Entities&, double);
void collisionSystem(Entities&);
void bounceSystem(Entities&);
void deadBlocksSystem(Entities&);

void scoreBlocksSystem(Entities&, int&);

void renderCharOnConsoleSystem(Entities&, ConsoleRenderWindow&); 
void renderStringOnConsoleSystem(Entities&, ConsoleRenderWindow&);