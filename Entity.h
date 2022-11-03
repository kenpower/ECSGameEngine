#pragma once
#include <chrono>
#include "CollisionRect.h"

struct Vector {
	double x, y;

	double lensq() const { return x * x + y * y; }

	bool operator>=(const Vector& b) const{
		return lensq() >= b.lensq() ? true : false;
	}
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

	void update(std::chrono::milliseconds);
	//virtual void update() = 0;
	void draw();

};

