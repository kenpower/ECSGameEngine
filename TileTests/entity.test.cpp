#include "pch.h"

#include "../Entity.h"
#include "mocks.h"
const double epsilon = 0.0001;


MockSprite sprite;

TEST(Level, EntityMoves) {

	Entity e(CollisionRect(10, 10, 1, 1), &sprite);
	
	e.vel = Vector{ 2, -3 };
	
	e.update(3800ms);
	e.update(200ms);
	
	EXPECT_NEAR(18, e.rect.x, epsilon);
	EXPECT_NEAR(-2, e.rect.y, epsilon);

}
