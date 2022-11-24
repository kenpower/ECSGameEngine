#include "ECS.h"
#include <vector>
#include "vector.h"
using namespace std;

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
	EntityID entity;
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
}

void getCollidableEntities(Components& components, vector<int>& movingColliders, vector<int>& allColliders) {
	for (auto& id_collisionBoxes : components.collisionBoxes) {
		EntityID id = id_collisionBoxes.first;

		auto mov = components.moveds[id];
		auto pos = components.positions[id];
		auto box = components.collisionBoxes[id];

		if (!pos or !box) continue;

		if (mov)
			movingColliders.push_back(id);

		allColliders.push_back(id);
	}
}

Collider makeCollider(PositionComponent* pos, CollisionBoxComponent* box) {
	Collider collider;
	collider.collisionRect = CollisionRect{ pos->x, pos->y, box->w, box->h };
	collider.position = pos;
	return collider;
}

void collisionSystem(Components& components) {
	vector<int> movingEntities;
	vector<int> allEntitiesWithCollisonBox;

	getCollidableEntities(components, movingEntities, allEntitiesWithCollisonBox);

	for (auto& movingEntity : movingEntities) 
		for (auto& otherEntity : allEntitiesWithCollisonBox) {
			if (otherEntity == movingEntity) continue;

			auto pos = components.positions[movingEntity];
			auto box = components.collisionBoxes[movingEntity];

			auto other_pos = components.positions[otherEntity];
			auto other_box = components.collisionBoxes[otherEntity];

			Collider movingCollider = makeCollider(pos, box);
			Collider otherCollider = makeCollider(other_pos, other_box);

			Vector mtv = getMinimumTranslationVector(movingCollider.collisionRect, otherCollider.collisionRect);
			bool intersecting = mtv.x != 0 or mtv.y != 0;
			if (intersecting) {
				resolveCollision(movingCollider, otherCollider, mtv);
				Vector axisAlignedNormal = getAxisAlignedNormal(mtv);

				components.collideds[movingEntity] = new CollidedComponent(otherEntity, axisAlignedNormal);
				components.collideds[otherEntity] = new CollidedComponent(movingEntity, -axisAlignedNormal);
			}
		}
	
}
