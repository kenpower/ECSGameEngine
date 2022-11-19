#pragma once
#include"ECS.h"
#include "ConsoleRenderWindow.h"

void movementSystem(Entities&, double);
void userControlSystem(Entities&, double);
void collisionSystem(Entities&);
void bounceSystem(Entities&);
void renderCharOnConsoleSystem(Entities&, ConsoleRenderWindow&); 
void renderStringOnConsoleSystem(Entities&, ConsoleRenderWindow&);