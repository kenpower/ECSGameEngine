#pragma once
#pragma once
#include"./ECSGameEngine/ECS.h"
#include"ECSGameEngine/ConsoleRenderWindow.h"


void movementSystem(Components&, double);
void userControlSystem(Components&, double);
void collisionSystem(Components&);

void renderCharOnConsoleSystem(Components& components, ConsoleRenderWindow& crw);
void renderStringOnConsoleSystem(Components& components, ConsoleRenderWindow& crw);
