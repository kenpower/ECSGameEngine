#pragma once

#include<string>

class CollisionRect
{
public:
	int x;
	int y;
	int w;
	int h;

	CollisionRect(int x, int y, int w, int h) :x(x), y(y), w(w), h(h) {}

	std::string toString() { return "Rect: x: " + std::to_string(x) + " y: " + std::to_string(y) + " w: " + std::to_string(w) + " h: " + std::to_string(h); }
};

bool isColliding(CollisionRect& a, CollisionRect& b, double epsilon = 0.001);

void resolveCollision(CollisionRect& a, CollisionRect& b);