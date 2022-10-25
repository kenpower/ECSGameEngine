#include "pch.h"

#include "../Level.h"

const double epsilon = 0.0001;

void EXPECT_VEQ(Vector a, Vector b) {

	EXPECT_NEAR(a.x, b.x, epsilon);
	EXPECT_NEAR(a.y, b.y, epsilon);
}

void EXPECT_VEQ(CollisionRect a, CollisionRect b) {

	EXPECT_NEAR(a.x, b.x, epsilon);
	EXPECT_NEAR(a.y, b.y, epsilon);
}

TEST(Level, CanGetEntityByName) {

	Level lvl(
		"A....."
		"B....."
		".....C" , 6,3);

	EXPECT_NEAR(0, lvl.getByName("A").rect.x, epsilon);
	EXPECT_NEAR(1, lvl.getByName("B").rect.y, epsilon);
	EXPECT_NEAR(5, lvl.getByName("C").rect.x, epsilon);

}

TEST(Level, CanUpdateAll) {

	Level lvl(
		"A....."
		"B....."
		".....C", 6, 3);

	lvl.getByName("A").vel = Vector{ 0.2, 0.0 };
	lvl.getByName("B").vel = Vector{ 0.0, 0.3 };
	lvl.getByName("C").vel = Vector{ -0.1, -0.2 };

	for (int i = 0; i < 100; i++)
		lvl.update(Seconds{ 0.1 });

	EXPECT_VEQ(lvl.getByName("A").rect, CollisionRect{ 2, 0, 1, 1 });
	EXPECT_VEQ(lvl.getByName("B").rect, CollisionRect{ 0, 4, 1, 1 });
	EXPECT_VEQ(lvl.getByName("C").rect, CollisionRect{ 4, 0, 1, 1 });


}

//TEST(Level, ResolveCollision) {
//
//	Level lvl(
//		"A....."
//		"B....."
//		"......", 6, 3);
//
//	lvl.getByName("A").vel = Vector{ 0.0, 0.0 };
//	lvl.getByName("B").vel = Vector{ 0.0, -0.5 };
//
//	lvl.update(Seconds{ 0.5 });
//
//	EXPECT_VEQ(lvl.getByName("A").rect, CollisionRect{ 0, 0, 1, 1 });
//	EXPECT_VEQ(lvl.getByName("B").rect, CollisionRect{ 0, 1, 1, 1 });
//}
