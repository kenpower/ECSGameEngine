#include "pch.h"

#include "../Entity.h"

const double epsilon = 0.0001;

TEST(Level, EntityMoves) {

	Entity e(CollisionRect(10, 10, 1, 1));
	
	e.vel = Vector{ 2, -3 };
	
	e.update(Seconds{ 3.8 });
	e.update(Seconds{ 0.2 });
	
	EXPECT_NEAR(18, e.rect.x, epsilon);
	EXPECT_NEAR(-2, e.rect.y, epsilon);

}
