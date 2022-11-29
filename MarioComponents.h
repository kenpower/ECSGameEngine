#pragma once

struct GravityComponent : public Component{ 
	GravityComponent(double f):force(f) {}
	double force;
};

struct JumpComponent : public Component {
	JumpComponent(double f) :force(f) {}
	double force;
};

struct OnFloorComponent : public Component {
};