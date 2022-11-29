#pragma once
#include<string>
#include"vector.h"

typedef int EntityID;

struct Component {
};

struct PositionComponent : public Component{ 
	PositionComponent(double x, double y) :x(x), y(y) {}
	double x, y;
};

struct VelocityComponent : public Component {
	VelocityComponent(double x, double y) :x(x), y(y) {}
	double x, y;
};

struct CharSpriteComponent : public Component {
	CharSpriteComponent(char c) :c(c) {}
	char c;
};

struct StringSpriteComponent : public Component {
	StringSpriteComponent(const char* c) :c(c) {}
	const char* c;
};

struct HasMovedComponent : public Component {
};

struct CollisionRectComponent : public Component {
	CollisionRectComponent(double w, double h) :w(w), h(h) {}
	CollisionRectComponent(double x, double y,double w, double h) :x(x), y(y), w(w), h(h) {}
	double x, y;
	double w, h;
};

struct HasCollidedComponent : public Component {
	HasCollidedComponent(EntityID other, Vector surfaceNormal = Vector{ 0, 0 }) :
		other(other),  surfaceNormal(surfaceNormal) {}

	Vector surfaceNormal;
	EntityID other;
};

