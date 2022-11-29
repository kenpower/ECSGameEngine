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

void jumpSystem(Components& components, double deltaSeconds) {

	for (auto& id_jmp : components.jumps) {
		EntityID id = id_jmp.first;

		auto jmp = id_jmp.second;
		auto pos = components.positions[id];
		auto col = components.collideds[id]; //only jump if 

		if (!jmp || !pos || !col) continue;

		// All they other Key codes are here https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		if (isKeyDown(VK_UP))
			pos->y += jmp->force * deltaSeconds;

		components.moveds[id] = new HasMovedComponent;
	}
}