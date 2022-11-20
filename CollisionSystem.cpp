#include "ECS.h"
#include "vector.h"

struct CollisionRect
{
	double x;
	double y;
	double w;
	double h;
};

double MTV_oneAxis(double aStart, double aEnd, double bStart, double bEnd) {
	double left = bStart - aEnd;
	double right = bEnd - aStart;

	if (left > 0 or right < 0) return 0;

	return fabs(left) < fabs(right) ? left : right;

}
//Minimum translation Vector (MTV) is the smallest distance needed to move an overlapping
//rectangle so they are no longer overlapping
Vector getMinimumTranslationVector(const CollisionRect& a, const CollisionRect& b) {
	double xMTV = MTV_oneAxis(a.x, a.x + a.w, b.x, b.x + b.w);
	double yMTV = MTV_oneAxis(a.y, a.y + a.h, b.y, b.y + b.h);

	return fabs(xMTV) < fabs(yMTV) ? Vector{xMTV,0} : Vector{0, yMTV};
}

struct Collider{
	CollisionRect collisionRect;
	PositionComponent* position;
	Entity* entity;
};

Vector getAxisAlignedNormal(Vector& v) {
	if (fabs(v.x) > fabs(v.y))
		return (v.x > 0) ? Vector{ 1, 0 } : Vector{ -1, 0 };
	return (v.y > 0) ? Vector{ 0, 1 } : Vector{ 0, -1 };
}
void resolveCollision(Collider& moving, Collider& other, Vector& mtv) {
	moving.position->x += mtv.x;
	moving.position->y += mtv.y;
	moving.collisionRect.x = moving.position->x;
	moving.collisionRect.y = moving.position->y;

	Vector axisAlignedNormal = getAxisAlignedNormal(mtv);

	auto crc = make_shared<CollisionResolvedComponent>(axisAlignedNormal.x, axisAlignedNormal.y);
	moving.entity->addComponent(crc);

	static auto collided = make_shared<CollidedComponent>();
	other.entity->addComponent(collided);
}

void getColliders(Entities& entities, vector<Collider>& movingColliders, vector<Collider>& allColliders) {
	for (auto& e : entities) {
		auto moved = dynamic_pointer_cast<MovedComponent>(e->getComponent(MovedComponent::NAME));
		auto box = dynamic_pointer_cast<CollisionBoxComponent>(e->getComponent(CollisionBoxComponent::NAME));
		auto pos = dynamic_pointer_cast<PositionComponent>(e->getComponent(PositionComponent::NAME));

		Collider collider;
		if (box && pos) {
			collider.collisionRect = CollisionRect{ pos->x, pos->y, box->w, box->h };
			collider.position = pos.get();
			collider.entity = e.get();
		}

		if (moved)
			movingColliders.push_back(collider);

		allColliders.push_back(collider);
	}
}

void collisionSystem(Entities& entities) {
	vector<Collider> movingColliders;
	vector<Collider> allColliders;

	getColliders(entities, movingColliders, allColliders);

	for (auto& moving : movingColliders) 
		for (auto& other : allColliders) {
			if (other.entity == moving.entity) continue;

			Vector mtv = getMinimumTranslationVector(moving.collisionRect, other.collisionRect);
			bool intersecting = mtv.x != 0 or mtv.y != 0;
			if (intersecting)
				resolveCollision(moving, other, mtv);
		}
	
}
