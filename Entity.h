#pragma once
#include "CollisionRect.h"

struct Vector {
	float x, y;
};

struct Seconds{
	float value;
};

class Entity
{
public:
	CollisionRect rect;
	Vector vel;
	
	Entity(const CollisionRect);

	void update(Seconds);
	//virtual void update() = 0;
	//virtual void draw() = 0;
};

