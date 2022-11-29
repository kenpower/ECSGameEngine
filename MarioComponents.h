#pragma once

struct GravityComponent : public Component{ 
	GravityComponent(double f):force(f) {}
	double force;
};