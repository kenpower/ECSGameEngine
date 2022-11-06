#pragma once
#include "entity.h"

class Ball :public Entity {
	Vector bounceNormal{ 0,0 };
	bool bounce{ false };
public:
	using Entity::Entity; //use parent constructor

	void collideWith(Entity* other, Vector adjustment) {
		bounce = true;

		if (fabs(adjustment.x) > fabs(adjustment.y))
			bounceNormal = (adjustment.x > 0) ? Vector{ 1, 0 } : Vector{ -1, 0 };
		else
			bounceNormal = (adjustment.y > 0) ? Vector{ 0,1 } : Vector{ 0,-1 };
	}

	void postCollisionUpdate() {
		if (bounce) {
			if (abs(lround(bounceNormal.x)) == 1) vel.x *= -1;
			if (abs(lround(bounceNormal.y)) == 1) vel.y *= -1;

		}
		bounce = false;
	}
};