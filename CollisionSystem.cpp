#include "ECS.h"
#include <vector>
#include "vector.h"
using namespace std;

double MTV_oneAxis(double aStart, double aEnd, double bStart, double bEnd) {
	double left = bStart - aEnd;
	double right = bEnd - aStart;

	if (left > 0 or right < 0) return 0;

	return fabs(left) < fabs(right) ? left : right;
}

//Minimum translation Vector (MTV) is the smallest distance needed to move an overlapping
//rectangle so they are no longer overlapping
Vector getMinimumTranslationVector(const CollisionRectComponent* a, const CollisionRectComponent* b) {
	double xMTV = MTV_oneAxis(a->x, a->x + a->w, b->x, b->x + b->w);
	double yMTV = MTV_oneAxis(a->y, a->y + a->h, b->y, b->y + b->h);

	return fabs(xMTV) < fabs(yMTV) ? Vector{xMTV,0} : Vector{0, yMTV};
}

Vector getAxisAlignedNormal(Vector& v) {
	if (fabs(v.x) > fabs(v.y))
		return (v.x > 0) ? Vector{ 1, 0 } : Vector{ -1, 0 };
	return (v.y > 0) ? Vector{ 0, 1 } : Vector{ 0, -1 };
}

void resolveCollision(int entityID, Vector& mtv, Components& components) {
	auto position = components.positions[entityID];
	position->x += mtv.x;
	position->y += mtv.y;
}

void getCollidableEntities(Components& components, vector<int>& movingColliders, vector<int>& allColliders) {
	for (auto& id_collisionRects : components.collisionRects) {
		EntityID id = id_collisionRects.first;

		auto mov = components.moveds[id];
		auto pos = components.positions[id];
		auto box = components.collisionRects[id];

		if (!pos or !box) continue;

		if (mov)
			movingColliders.push_back(id);

		allColliders.push_back(id);
	}
}

CollisionRectComponent* getUpdatedCollisionRectFor(int entityID, Components& components) {
	auto pos = components.positions[entityID];
	auto rect = components.collisionRects[entityID];
	rect->x = pos->x;
	rect->y = pos->y;
	return rect;
}

void collisionSystem(Components& components) {
	vector<int> movingEntities;
	vector<int> allEntitiesWithCollisonBox;

	getCollidableEntities(components, movingEntities, allEntitiesWithCollisonBox);

	for (auto& movingEntity : movingEntities) 
		for (auto& otherEntity : allEntitiesWithCollisonBox) {
			if (otherEntity == movingEntity) continue;

			auto movingRect = getUpdatedCollisionRectFor(movingEntity, components);
			auto otherRect = getUpdatedCollisionRectFor(otherEntity, components);

			Vector mtv = getMinimumTranslationVector(movingRect, otherRect);
			
			bool intersecting = mtv.x != 0 or mtv.y != 0;
			if (intersecting) {

				resolveCollision(movingEntity,  mtv, components);
				
				Vector axisAlignedNormal = getAxisAlignedNormal(mtv);
				components.collideds[movingEntity] = new CollidedComponent(otherEntity, axisAlignedNormal);
				components.collideds[otherEntity] = new CollidedComponent(movingEntity, -axisAlignedNormal);
			}
		}
}
