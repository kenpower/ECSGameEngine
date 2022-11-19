#include "ECS.h"
#include "vector.h"

struct CollisionRect
{
	double x;
	double y;
	double w;
	double h;
};


Vector getOverLapBetween(CollisionRect& a, CollisionRect& b) {

	double halfWidthA = a.w / 2;
	double halfWidthB = b.w / 2;
	double halfHeightA = a.h / 2;
	double halfHeightB = b.h / 2;

	double distancex = (a.x + halfWidthA) - (b.x + halfWidthB);
	double distancey = (a.y + halfHeightA) - (b.y + halfHeightB);

	// can't have negative distance
	if (distancex < 0) distancex = -distancex;
	if (distancey < 0) distancey = -distancey;

	// calculate the minimum distance before a collision
	double minDistX = halfWidthA + halfWidthB;
	double minDistY = halfHeightA + halfHeightB;

	// calculate the depth of the collision
	double overlapX = minDistX - distancex;
	double overlapY = minDistY - distancey;

	Vector smallestOverlap{ 0,0 };

	if (overlapX > 0 && overlapY > 0)
		if (overlapX < overlapY)
			smallestOverlap.x = (a.x > b.x) ? overlapX : -overlapX;
		else
			smallestOverlap.y = (a.y > b.y) ? overlapY : -overlapY;

	return smallestOverlap;
}

void resolveCollision(Entity* moving, Entity* other, PositionComponent* position, Vector& overlap) {
	position->x += overlap.x;
	position->y += overlap.y;

	Vector bn;
	if (fabs(overlap.x) > fabs(overlap.y))
		bn = (overlap.x > 0) ? Vector{ 1, 0 } : Vector{ -1, 0 };
	else
		bn = (overlap.y > 0) ? Vector{ 0, 1 } : Vector{ 0, -1 };

	auto crc = make_shared<CollisionResolvedComponent>(bn.x, bn.y);
	moving->addComponent(crc);

	static auto collided = make_shared<CollidedComponent>();
	other->addComponent(collided);
}

void collisionSystem(Entities& entities) {

	for (auto& e : entities) {
		auto moved = dynamic_pointer_cast<MovedComponent>(e->getComponent(MovedComponent::NAME));
		auto box = dynamic_pointer_cast<CollisionBoxComponent>(e->getComponent(CollisionBoxComponent::NAME));
		auto pos = dynamic_pointer_cast<PositionComponent>(e->getComponent(PositionComponent::NAME));
		if (!(moved && box && pos)) continue;
		for (auto& other : entities) {
			if (other == e) continue;
			auto other_pos = dynamic_pointer_cast<PositionComponent>(other->getComponent(PositionComponent::NAME));
			auto other_box = dynamic_pointer_cast<CollisionBoxComponent>(other->getComponent(CollisionBoxComponent::NAME));
			if (!other_pos || !other_box) continue;

			CollisionRect movingRect{ pos->x, pos->y, box->w, box->h };
			CollisionRect otherRect{ other_pos->x, other_pos->y, other_box->w, other_box->h };

			Vector overlap = getOverLapBetween(movingRect, otherRect);
			bool intersecting = overlap.x != 0 or overlap.y != 0;
			if (intersecting)
				resolveCollision(e.get(), other.get(), pos.get(), overlap);

		}
	}
}
