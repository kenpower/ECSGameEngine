#pragma once
#include<string>
#include"vector.h"

typedef int EntityID;

struct Component {
};

struct PositionComponent { 
	double x, y;
};

struct VelocityComponent  {
	double x, y;
};

struct CharSpriteComponent {
	char c;
};

struct StringSpriteComponent {
	const char* c;
};

struct HasMovedComponent  {
};

struct CollisionRectComponent {
	CollisionRectComponent(double w, double h) :w{ w }, h{ h } {}
	CollisionRectComponent(double x, double y, double w, double h) :x(x), y(y), w{ w }, h{ h } {}
	double x, y;
	double w, h;
};

struct HasCollidedComponent {
	HasCollidedComponent(EntityID other, Vector surfaceNormal = Vector{ 0, 0 }) :
		other(other),  surfaceNormal(surfaceNormal) {}

	Vector surfaceNormal;
	EntityID other;
};

