#define NOMINMAX //https://stackoverflow.com/a/2789510/488802
#include<windows.h>
#include"BreakoutSystems.h"

void movementSystem(Entities& entities, double deltaSeconds) {
	static auto movedComponent = make_shared<MovedComponent>();
	
	for (auto& e : entities) {
		//auto* vel = dynamic_cast<VelocityComponent*>(e->getComponent(VelocityComponent::NAME));
		//auto* pos = dynamic_cast<PositionComponent*>(e->getComponent(PositionComponent::NAME));
		auto vel = e->getComponent<VelocityComponent>();
		auto pos = e->getComponent<PositionComponent>();

		if (vel && pos) {
			pos->x += vel->x * deltaSeconds;
			pos->y += vel->y * deltaSeconds;
			e->addComponent(movedComponent);
		}
	}
}

bool isKeyDown(int keyCode) {
	const short leftMostBit = (short)0x8000; //most significant bit is set if key is down
	return  leftMostBit & GetAsyncKeyState((unsigned char)(keyCode));
}

void userControlSystem(Entities& entities, double deltaSeconds) {
	static auto movedComponent = make_shared<MovedComponent>();
	for (auto& e : entities) {
		auto lrc = dynamic_pointer_cast<LeftRightControlComponent>(e->getComponent(LeftRightControlComponent::NAME));
		auto pos = dynamic_pointer_cast<PositionComponent>(e->getComponent(PositionComponent::NAME));
		if (!lrc || !pos) continue; 

		// All they other Key codes are here https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		if (isKeyDown(VK_RIGHT)) 
			pos->x += lrc->speed * deltaSeconds;
		if (isKeyDown(VK_LEFT))
			pos->x -= lrc->speed * deltaSeconds;
		e->addComponent(movedComponent);
	}
}

void bounceSystem(Entities& entities) {
	for (auto& e : entities) {
		auto bounce = dynamic_pointer_cast<BounceComponent>(e->getComponent(BounceComponent::NAME));
		auto vel = dynamic_pointer_cast<VelocityComponent>(e->getComponent(VelocityComponent::NAME));
		auto collision = dynamic_pointer_cast<CollidedComponent>(e->getComponent(CollidedComponent::NAME));
		if (!bounce || !vel || !collision) continue;
		
		if (abs(lround(collision->surfaceNormal.x)) == 1) vel->x *= -1; //vertical   wall
		if (abs(lround(collision->surfaceNormal.y)) == 1) vel->y *= -1; //horizontal wall
		
	}
}

void renderCharOnConsoleSystem(Entities& entities, ConsoleRenderWindow& crw) {
	for (auto& e : entities) {
		auto sprite = dynamic_pointer_cast<CharSpriteComponent>(e->getComponent(CharSpriteComponent::NAME));
		auto pos = dynamic_pointer_cast<PositionComponent>(e->getComponent(PositionComponent::NAME));
		if (!sprite || !pos) continue;

		crw.Draw(lround(pos->x), lround(pos->y), sprite->c);
		
	}
}

void renderStringOnConsoleSystem(Entities& entities, ConsoleRenderWindow& crw) {
	for (auto& e : entities) {
		auto sprite = dynamic_pointer_cast<StringSpriteComponent>(e->getComponent(StringSpriteComponent::NAME));
		auto pos = dynamic_pointer_cast<PositionComponent>(e->getComponent(PositionComponent::NAME));
		if (!sprite || !pos) continue;

		for (int i = 0; sprite->c[i] != '\0'; i++)
			crw.Draw(lround(pos->x + i), lround(pos->y), sprite->c[i]);
	}
}

void deadBlocksSystem(Entities& entities) {
	for (auto e = entities.begin(); e != entities.end();) 
	//can't use Range-based loop as we need to remove some items
	{

		auto collided = dynamic_pointer_cast<CollidedComponent>((*e)->getComponent(CollidedComponent::NAME));
		auto block = dynamic_pointer_cast<DeleteAfterCollisionComponent>((*e)->getComponent(DeleteAfterCollisionComponent::NAME));

		if (collided && block)
			e = entities.erase(e);
		else
			++e;
	}
}

void scoreBlocksSystem(Entities& entities, int& gameScore) {
	for (auto& e : entities) {
		auto collided = dynamic_pointer_cast<CollidedComponent>(e->getComponent(CollidedComponent::NAME));
		auto score = dynamic_pointer_cast<ScoreWhenHitBlockComponent>(e->getComponent(ScoreWhenHitBlockComponent::NAME));

		if (!collided || !score) continue;
		
		gameScore+=score->score;

	}
}

void ballPaddleBounce(Entities& entities, int& gameScore) {
	for (auto& e : entities) {
		auto collided = dynamic_pointer_cast<CollidedComponent>(e->getComponent(CollidedComponent::NAME));
		auto score = dynamic_pointer_cast<ScoreWhenHitBlockComponent>(e->getComponent(ScoreWhenHitBlockComponent::NAME));

		if (!collided || !score) continue;

		gameScore += score->score;
	}
}