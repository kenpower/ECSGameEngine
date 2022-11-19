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