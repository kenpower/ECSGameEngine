#pragma once
#include"./ECSGameEngine/ECS.h"
//#include "./ECSGameEngine/ConsoleRenderWindow.h"

void gravitySystem(Components&, double);
void jumpSystem(Components& components, double deltaSeconds);
void hitWallSystem(Components & components);
	
