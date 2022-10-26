#include "CollisionRect.h"

bool isColliding(CollisionRect& a, CollisionRect& b, double epsilon) {

	if (a.x + a.w < b.x + epsilon) return false;
	if (a.x + epsilon > b.x + b.w) return false;
	if (a.y + a.h < b.y + epsilon) return false;
	if (a.y + epsilon > b.y + b.h) return false;

	return true;
}

void resolveCollision(CollisionRect& a, CollisionRect& b) {
	{
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

		if (overlapX <= 0 || overlapY <= 0) return;

		// resolve the collision
		if (overlapX < overlapY) {
			a.x += (a.x > b.x) ? overlapX : -overlapX;
		}
		else {
			a.y += (a.y > b.y) ? overlapY : -overlapY;
		}
	}
}