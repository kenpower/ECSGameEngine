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

struct Collider{
	CollisionRect collisionRect;
	PositionComponent* position;
	Entity* entity;
};

void resolveCollision(Collider& moving, Collider& other, Vector& overlap) {
	moving.position->x += overlap.x;
	moving.position->y += overlap.y;

	Vector bn;
	if (fabs(overlap.x) > fabs(overlap.y))
		bn = (overlap.x > 0) ? Vector{ 1, 0 } : Vector{ -1, 0 };
	else
		bn = (overlap.y > 0) ? Vector{ 0, 1 } : Vector{ 0, -1 };

	auto crc = make_shared<CollisionResolvedComponent>(bn.x, bn.y);
	moving.entity->addComponent(crc);

	static auto collided = make_shared<CollidedComponent>();
	other.entity->addComponent(collided);
}


void collisionSystem(Entities& entities) {
	vector<Collider> movingColliders;
	vector<Collider> allColliders;

	for (auto& e : entities) {
		auto moved = dynamic_pointer_cast<MovedComponent>(e->getComponent(MovedComponent::NAME));
		auto box = dynamic_pointer_cast<CollisionBoxComponent>(e->getComponent(CollisionBoxComponent::NAME));
		auto pos = dynamic_pointer_cast<PositionComponent>(e->getComponent(PositionComponent::NAME));

		Collider collider;
		if (box && pos){
			collider.collisionRect = CollisionRect{ pos->x, pos->y, box->w, box->h };
			collider.position = pos.get();
			collider.entity = e.get();
		}

		if (moved)
			movingColliders.push_back(collider);
		
		allColliders.push_back(collider);
	}

	for (auto& moving : movingColliders) {
		for (auto& other : allColliders) {
			if (other.entity == moving.entity) continue;

			Vector overlap = getOverLapBetween(moving.collisionRect, other.collisionRect);
			bool intersecting = overlap.x != 0 or overlap.y != 0;
			if (intersecting)
				resolveCollision(moving, other, overlap);
		}
	}
}
