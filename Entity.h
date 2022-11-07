#pragma once
#include <chrono>
#include "vector.h"
#include "CollisionRect.h"

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
	bool active{ true };

	Entity(const CollisionRect r, Sprite* s) :rect(r), sprite(s) {}

	virtual void update(std::chrono::milliseconds);
	virtual void postCollisionUpdate() {};
	virtual void collideWith(Entity*, Vector) {}

	void draw();
};

