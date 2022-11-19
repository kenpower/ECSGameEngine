#include<windows.h>
#include"BreakoutSystems.h"

void movementSystem(Entities& entities, double deltaSeconds) {
	static auto movedComponent = make_shared<MovedComponent>();
	
	for (auto e : entities) {
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
	short leftMostBit = 0x8000;
	//most significant bit is set if key is down
	return  leftMostBit & GetAsyncKeyState((unsigned char)(keyCode));
}

void userControlSystem(Entities& entities, double deltaSeconds) {
	static auto movedComponent = make_shared<MovedComponent>();
	for (auto e : entities) {
		auto lrc = dynamic_pointer_cast<LeftRightControlComponent>(e->getComponent(LeftRightControlComponent::NAME));
		auto pos = dynamic_pointer_cast<PositionComponent>(e->getComponent(PositionComponent::NAME));
		if (!lrc || !pos) continue; 

		if (isKeyDown(VK_RIGHT))
			pos->x += lrc->speed * deltaSeconds;
		if (isKeyDown(VK_LEFT))
			pos->x -= lrc->speed * deltaSeconds;
		e->addComponent(movedComponent);
	}
}

void bounceSystem(Entities& entities) {
	for (auto e : entities) {
		auto bounce = dynamic_pointer_cast<BounceComponent>(e->getComponent(BounceComponent::NAME));
		auto vel = dynamic_pointer_cast<VelocityComponent>(e->getComponent(VelocityComponent::NAME));
		auto collision = dynamic_pointer_cast<CollisionResolvedComponent>(e->getComponent(CollisionResolvedComponent::NAME));
		if (!bounce || !vel || !collision) continue;
		
		if (abs(lround(collision->x)) == 1) vel->x *= -1; //vertical   wall
		if (abs(lround(collision->y)) == 1) vel->y *= -1; //horizontal wall
		
	}
}

void renderCharOnConsoleSystem(Entities& entities, ConsoleRenderWindow& crw) {
	for (auto e : entities) {
		auto sprite = dynamic_pointer_cast<CharSpriteComponent>(e->getComponent(CharSpriteComponent::NAME));
		auto pos = dynamic_pointer_cast<PositionComponent>(e->getComponent(PositionComponent::NAME));
		if (!sprite || !pos) continue;

		crw.Draw(round(pos->x), round(pos->y), sprite->c);
		
	}
}

void renderStringOnConsoleSystem(Entities& entities, ConsoleRenderWindow& crw) {
	for (auto e : entities) {
		auto sprite = dynamic_pointer_cast<StringSpriteComponent>(e->getComponent(StringSpriteComponent::NAME));
		auto pos = dynamic_pointer_cast<PositionComponent>(e->getComponent(PositionComponent::NAME));
		if (!sprite || !pos) continue;

		for (int i = 0; sprite->c[i] != '\0'; i++)
			crw.Draw(round(pos->x + i), round(pos->y), sprite->c[i]);
	}
}