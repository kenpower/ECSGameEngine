#define NOMINMAX //https://stackoverflow.com/a/2789510/488802
#include<windows.h>
#include"MarioSystems.h"

void gravitySystem(Components& components, double deltaSeconds) {

	for (auto& id_vel : components.gravities) {
		EntityID id = id_vel.first;

		auto gravity = id_vel.second;
		auto vel = components.velocitys[id];

		if (gravity && vel) {
			vel->y += gravity->force * deltaSeconds;
		}
	}
}

void jumpSystem(Components& components, double deltaSeconds) {

	for (auto& id_jmp : components.jumps) {
		EntityID id = id_jmp.first;

		auto jmp = id_jmp.second;
		auto vel = components.velocitys[id];
		auto col = components.collideds[id]; //only jump if colliding

		if (!jmp || !vel || !col) continue;

		// All they other Key codes are here https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		if (isKeyDown(VK_UP))
			vel->y = - jmp->force;

	}
}

void hitWallSystem(Components& components) {

	for (auto& id_col : components.collideds) {
		EntityID id = id_col.first;

		auto col = id_col.second;
		auto normal = col->surfaceNormal;
		auto vel = components.velocitys[id];
		
		if (!col || !vel) continue;
			
		if (normal.x == 1 && vel->x <  0 ) vel->x = 0;
		if (normal.x == -1 && vel->x > 0) vel->x = 0;
		if (normal.y == 1 && vel->y < 0) vel->y = 0;
		if (normal.y == -1 && vel->y > 0) vel->y = 0;
	}
}