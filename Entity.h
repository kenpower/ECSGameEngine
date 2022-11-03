#pragma once
#include "CollisionRect.h"

struct Vector {
	double x, y;
};

struct Seconds{
	double value;
};

class Sprite {
public:
	virtual void draw(int x, int y) = 0;
};

class Entity
{
public:
	CollisionRect rect;
	Vector vel{0,0};
	Sprite* sprite;
	
	Entity(const CollisionRect r, Sprite* s) :rect(r), sprite(s) {}

	void update(Seconds);
	//virtual void update() = 0;
	void draw();

};

