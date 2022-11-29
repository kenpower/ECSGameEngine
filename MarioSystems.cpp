#define NOMINMAX //https://stackoverflow.com/a/2789510/488802
#include<windows.h>
#include"MarioSystems.h"

void gravitySystem(Components& components, double deltaSeconds) {

	for (auto& id_vel : components.gravities) {
		EntityID id = id_vel.first;

		auto gravity = id_vel.second;
		auto pos = components.positions[id];

		if (gravity && pos) {
			pos->y += gravity->force * deltaSeconds;
			components.moveds[id] = new HasMovedComponent;
		}
	}
}