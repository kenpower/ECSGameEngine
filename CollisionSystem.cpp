#include "ECS.h"
#include "vector.h"

struct CollisionRect
{
	double x;
	double y;
	double w;
	double h;
};

bool resolveCollision(CollisionRect& a, CollisionRect& b, Vector& adjustment) {

	// calculate the distance vector
	double dx = (a.x + a.w / 2) - (b.x + b.w / 2);
	double dy = (a.y + a.h / 2) - (b.y + b.h / 2);

	// can't have negative distance
	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;

	// calculate the minimum distance before a collision
	double minDistX = a.w / 2 + b.w / 2;
	double minDistY = a.h / 2 + b.h / 2;

	// calculate the depth of the collision
	double overlapX = minDistX - dx;
	double overlapY = minDistY - dy;

	if (overlapX <= 0 || overlapY <= 0) return false;

	// resolve the collision
	if (overlapX < overlapY) {
		adjustment.x = (a.x > b.x) ? overlapX : -overlapX;
		//a.x += adjustment.x;
	}
	else {
		adjustment.y = (a.y > b.y) ? overlapY : -overlapY;
		//a.y += adjustment.y;
	}

	return true;
}


void collisionSystem(Entities& entities) {
	static auto collided = make_shared<CollidedComponent>();
	for (auto& e : entities) {
		auto moved = dynamic_pointer_cast<MovedComponent>(e->getComponent(MovedComponent::NAME));
		auto box = dynamic_pointer_cast<CollisionBoxComponent>(e->getComponent(CollisionBoxComponent::NAME));
		auto pos = dynamic_pointer_cast<PositionComponent>(e->getComponent(PositionComponent::NAME));
		if (moved && box && pos)
			for (auto& other : entities) {
				if (other == e) continue;
				auto other_pos = dynamic_pointer_cast<PositionComponent>(other->getComponent(PositionComponent::NAME));
				auto other_box = dynamic_pointer_cast<CollisionBoxComponent>(other->getComponent(CollisionBoxComponent::NAME));
				if (other_pos && other_box) {
					CollisionRect moving{ pos->x, pos->y, box->w, box->h };
					CollisionRect other_r{ other_pos->x, other_pos->y, other_box->w, other_box->h };
					Vector adj{ 0,0 };
					bool collision = resolveCollision(moving, other_r, adj);
					if (collision) {
						pos->x += adj.x;
						pos->y += adj.y;
						Vector bn;
						if (fabs(adj.x) > fabs(adj.y))
							bn = (adj.x > 0) ? Vector{ 1, 0 } : Vector{ -1, 0 };
						else
							bn = (adj.y > 0) ? Vector{ 0,1 } : Vector{ 0,-1 };
						auto crc = make_shared<CollisionResolvedComponent>(bn.x, bn.y);
						e->addComponent(crc);
						other->addComponent(collided);
					}
				}
			}
	}
}