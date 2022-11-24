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
}

void getColliders(Components& components, vector<int>& movingColliders, vector<int>& allColliders) {
	for (auto& id_mov : components.moveds) {
		EntityID id = id_mov.first;

		auto mov = id_mov.second;
		auto pos = components.positions[id];
		auto box = components.collisionBoxes[id];

		Collider collider;
		if (box && pos) {
			collider.collisionRect = CollisionRect{ pos->x, pos->y, box->w, box->h };
			collider.position = pos;
		}

		if (mov)
			movingColliders.push_back(id);

		allColliders.push_back(id);
	}
}

void collisionSystem(Components& components) {
	vector<int> movingColliders;
	vector<int> allColliders;

	getColliders(components, movingColliders, allColliders);

	for (auto& moving : movingColliders) 
		for (auto& other : allColliders) {
			if (other == moving) continue;

			auto pos = components.positions[moving];
			auto box = components.collisionBoxes[moving];


			Vector mtv = getMinimumTranslationVector(moving.collisionRect, other.collisionRect);
			bool intersecting = mtv.x != 0 or mtv.y != 0;
			if (intersecting) {
				resolveCollision(moving, other, mtv);
				Vector axisAlignedNormal = getAxisAlignedNormal(mtv);


				//auto ccForMoving = make_shared<CollidedComponent>(other.entity, axisAlignedNormal);
				//moving.entity->addComponent(ccForMoving);
				components[id]

				//auto ccForOther = make_shared<CollidedComponent>(moving.entity, -axisAlignedNormal);
				//other.entity->addComponent(ccForOther);
			}
		}
	
}
