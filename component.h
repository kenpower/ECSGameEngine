#pragma once
#include<string>
#include"Vector.h"

typedef int EntityID;

struct Component {
	virtual std::string name() = 0;
};

struct PositionComponent : public Component{ 
	//TOTO find a better way of doing this
	// need to be able to get the name of the class(static)
	// and polymorphically from the pointer
	// better would be not to need name

	static constexpr const char* NAME = "position";
	virtual std::string name() { return std::string(NAME); }

	PositionComponent(double x, double y) :x(x), y(y) {}
	double x, y;
};

struct VelocityComponent : public Component {
	static constexpr const char* NAME = "velocity";
	virtual std::string name() { return std::string(NAME); }

	VelocityComponent(double x, double y) :x(x), y(y) {}
	double x, y;
};

struct CharSpriteComponent : public Component {
	static constexpr const char* NAME = "charsprite";
	virtual std::string name() { return std::string(NAME); }

	CharSpriteComponent(char c) :c(c) {}
	char c;
};

struct StringSpriteComponent : public Component {
	static constexpr const char* NAME = "stringsprite";
	virtual std::string name() { return std::string(NAME); }

	StringSpriteComponent(const char* c) :c(c) {}
	const char* c;
};

struct LeftRightControlComponent : public Component {
	static constexpr const char* NAME = "leftrightcontrol";
	virtual std::string name() { return std::string(NAME); }

	LeftRightControlComponent(double speed) :speed(speed) {}
	double speed;
};

struct MovedComponent : public Component {
	static constexpr const char* NAME = "moved";
	virtual std::string name() { return std::string(NAME); }
};

struct CollisionBoxComponent : public Component {
	static constexpr const char* NAME = "collisionbox";
	virtual std::string name() { return std::string(NAME); }

	CollisionBoxComponent(double w = 1, double h =1) :w(w), h(h) {}
	const double w, h;
};

struct BounceComponent : public Component {
	static constexpr const char* NAME = "bouncecollision";
	virtual std::string name() { return std::string(NAME); }

	BounceComponent() {}
};

struct DeleteAfterCollisionComponent : public Component {
	static constexpr const char* NAME = "block";
	virtual std::string name() { return std::string(NAME); }

	DeleteAfterCollisionComponent() {}
};

struct CollidedComponent : public Component {
	static constexpr const char* NAME = "collided";
	virtual std::string name() { return std::string(NAME); }

	CollidedComponent(EntityID other, Vector surfaceNormal = Vector{ 0, 0 }) :
		other(other),  surfaceNormal(surfaceNormal) {}

	Vector surfaceNormal;
	EntityID other;
};

struct ScoreWhenHitBlockComponent : public Component {
	static constexpr const char* NAME = "scorewhenhit";
	virtual std::string name() { return std::string(NAME); }
	int score;
	ScoreWhenHitBlockComponent(int score = 1 ):score(score) {}
};

struct PaddleBounceComponent : public Component {
	static constexpr const char* NAME = "paddlebounce";
	virtual std::string name() { return std::string(NAME); }

	PaddleBounceComponent(double centre):paddleCentre(centre) {};
	double paddleCentre;
};