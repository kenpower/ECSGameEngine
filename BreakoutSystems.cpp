#define NOMINMAX //https://stackoverflow.com/a/2789510/488802
#include<windows.h>
#include"BreakoutSystems.h"

void movementSystem(Components& components, double deltaSeconds) {
	
	for (auto& id_vel : components.velocitys) {
		EntityID id = id_vel.first;

		auto vel = id_vel.second;
		auto pos = components.positions[id];

		if (vel && pos) {
			pos->x += vel->x * deltaSeconds;
			pos->y += vel->y * deltaSeconds;
			components.moveds[id]= new MovedComponent;
		}
	}
}

bool isKeyDown(int keyCode) {
	const short leftMostBit = (short)0x8000; //most significant bit is set if key is down
	return  leftMostBit & GetAsyncKeyState((unsigned char)(keyCode));
}

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
		components.moveds[id] = new MovedComponent;
	}
}

void bounceSystem(Components& components) {
	for (auto& id_bounce : components.collideds) {
		EntityID id = id_bounce.first;
		auto bounce = id_bounce.second;

		auto collision = components.collideds[id];
		auto vel = components.velocitys[id];
		auto pos = components.positions[id];
		auto paddlePos = components.positions[collision->other];


		if (!bounce || !vel || !collision) continue;

		if (abs(lround(collision->surfaceNormal.x)) == 1) vel->x *= -1; //vertical   wall
		if (abs(lround(collision->surfaceNormal.y)) == 1) vel->y *= -1; //horizontal wall

		if (components.paddleBounces[collision->other] && pos && paddlePos) {
			double ballx = pos->x;
			double paddleCentre = paddlePos->x + 3;
			double ballOffset = (paddleCentre - ballx)/3.0;
			const double PI = 3.141;
			double angle = PI / 2 + ballOffset * PI / 3;
			double ballSpeed = sqrt(vel->x * vel->x + vel->y * vel->y);
			vel->x = ballSpeed * cos(angle);
			vel->y = ballSpeed * sin(angle) * -1;
			return;

		}
		
		
	}
}

void renderCharOnConsoleSystem(Components& components, ConsoleRenderWindow& crw) {
	for (auto& id_charSprite : components.charSprites) {
		EntityID id = id_charSprite.first;
		auto charSprite = id_charSprite.second;

		auto pos = components.positions[id];

		if (!charSprite || !pos) continue;

		crw.Draw(lround(pos->x), lround(pos->y), charSprite->c);
	}
}

void renderStringOnConsoleSystem(Components& components, ConsoleRenderWindow& crw) {
	for (auto& id_stringSprite : components.stringSprites) {
		EntityID id = id_stringSprite.first;
		auto stringSprite = id_stringSprite.second;

		auto pos = components.positions[id];
		if (!stringSprite || !pos) continue;

		for (int i = 0; stringSprite->c[i] != '\0'; i++)
			crw.Draw(lround(pos->x + i), lround(pos->y), stringSprite->c[i]);
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

void ballPaddleBounce(Components& components) {
}