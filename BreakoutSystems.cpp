#define NOMINMAX //https://stackoverflow.com/a/2789510/488802
#include<windows.h>
#include"BreakoutSystems.h"

void userControlSystem(Components& components, double deltaSeconds) {

	for (auto& id_lrc : components.leftRightControls) {
		EntityID id = id_lrc.first;

		auto lrc = id_lrc.second;
		auto pos = components.positions[id];

		if (!lrc || !pos) continue;

		// All they other Key codes are here https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		if (isKeyDown(VK_RIGHT)) 
			pos->x += lrc->speed * deltaSeconds;
		if (isKeyDown(VK_LEFT))
			pos->x -= lrc->speed * deltaSeconds;
		components.moveds[id] = new HasMovedComponent;
	}
}

void bounceSystem(Components& components) {
	for (auto& id_bounce : components.collideds) {
 		EntityID id = id_bounce.first;
		auto bounce = id_bounce.second;

		auto collision = components.collideds[id];
		auto vel = components.velocitys[id];
		auto pos = components.positions[id];

		if (!bounce || !vel || !collision) continue;

		auto paddlePos = components.positions[collision->other];
		auto paddleBounce = components.paddleBounces[collision->other];

		//if (!paddlePos || !paddleBounce) continue;


		if (abs(lround(collision->surfaceNormal.x)) == 1) vel->x *= -1; //vertical   wall
		if (abs(lround(collision->surfaceNormal.y)) == 1) vel->y *= -1; //horizontal wall

		if (paddleBounce && pos && paddlePos) {
			double ballx = pos->x;
			double paddleCentre = paddlePos->x + paddleBounce->paddleCentre;
			double percentDeflection = (paddleCentre - ballx)/ paddleBounce->paddleCentre;
			const double PI = 3.141;
			const double straightUp = PI / 2;
			const double maxDeflection = PI / 3;

			double angle = straightUp + percentDeflection * maxDeflection;
			double ballSpeed = sqrt(vel->x * vel->x + vel->y * vel->y);
			vel->x = ballSpeed * cos(angle);
			vel->y = ballSpeed * sin(angle) * -1;
		}
	}
}



void deadBlocksSystem(Components& components) {
	for (auto iter = components.collideds.begin(); iter != components.collideds.end();) {
	//can't use Range-based loop as we need to remove items
		EntityID id = iter->first;
		
		auto collided = iter->second;
		++iter;

		auto del = components.deleteAfterCollisions[id];

		if (collided && del)
			components.deleteEntity(id);
		
	}
}

void scoreBlocksSystem(Components& components, int& gameScore) {
	for (auto& id_col : components.collideds) {
		EntityID id = id_col.first;

		auto collided = id_col.second;
		auto score = components.scoreWhenHitBlock[id];

		if (!collided || !score) continue;
		
		gameScore+=score->score;

	}
}