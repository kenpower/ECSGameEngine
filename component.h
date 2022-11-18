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
	char speed;
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

struct CollisionResolvedComponent : public Component {
	static constexpr const char* NAME = "resolvedcollision";
	virtual std::string name() { return std::string(NAME); }

	CollisionResolvedComponent(double x, double y) :x(x), y(y) {}
	double x, y;
};

struct BounceComponent : public Component {
	static constexpr const char* NAME = "bouncecollision";
	virtual std::string name() { return std::string(NAME); }

	BounceComponent() {}
};

struct BlockComponent : public Component {
	static constexpr const char* NAME = "block";
	virtual std::string name() { return std::string(NAME); }

	BlockComponent() {}
};

struct CollidedComponent : public Component {
	static constexpr const char* NAME = "collided";
	virtual std::string name() { return std::string(NAME); }

	CollidedComponent() {}
};