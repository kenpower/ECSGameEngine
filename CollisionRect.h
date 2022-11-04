#pragma once

#include<string>
#include "vector.h"

class CollisionRect
{
public:
	double x;
	double y;
	double w;
	double h;

	CollisionRect(double x, double y, double w, double h) :x(x), y(y), w(w), h(h) {}

	std::string toString() { return "Rect: x: " + std::to_string(x) + " y: " + std::to_string(y) + " w: " + std::to_string(w) + " h: " + std::to_string(h); }
};

bool isColliding(CollisionRect& a, CollisionRect& b, double epsilon = 0.001);

bool resolveCollision(CollisionRect& a, CollisionRect& b, Vector& adjustment);