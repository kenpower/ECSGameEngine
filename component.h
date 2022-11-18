#pragma once
#include<string>

struct Component {
	virtual std::string name() = 0;
};

struct PositionComponent : public Component {
	//TOTO find a better way of doing this
	// need to be sble to get the name of the class(static)
	// and polymorphically from the pointer
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

struct LeftRightControlComponent : public Component {
	static constexpr const char* NAME = "leftrightcontrol";
	virtual std::string name() { return std::string(NAME); }

	LeftRightControlComponent(double speed) :speed(speed) {}
	char speed;
};

struct UnitCollisionBoxComponent : public Component {
	static constexpr const char* NAME = "unitcollisionbox";
	virtual std::string name() { return std::string(NAME); }

	const double w=1, h=1;
};

struct CollisionResolvedComponent : public Component {
	static constexpr const char* NAME = "resolvedcollision";
	virtual std::string name() { return std::string(NAME); }

	CollisionResolvedComponent(double x, double y) :x(x), y(y) {}
	double x, y;
};