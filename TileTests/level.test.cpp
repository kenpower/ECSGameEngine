#include "pch.h"

#include "../Level.h"

#include "mocks.h"

const double epsilon = 0.0001;
MockSpriteFactory spriteFactory;
MockSprite sprite;

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
		".....C" , 6,3, &spriteFactory);

	EXPECT_NEAR(0, lvl.getByName("A0-0")->rect.x, epsilon);
	EXPECT_NEAR(1, lvl.getByName("B0-1")->rect.y, epsilon);
	EXPECT_NEAR(5, lvl.getByName("C5-2")->rect.x, epsilon);

}

TEST(Level, CanUpdateAll) {

	Level lvl(
		"A....."
		"B....."
		".....C", 6, 3, &spriteFactory);

	lvl.getByName("A0-0")->vel = Vector{ 0.2, 0.0 };
	lvl.getByName("B0-1")->vel = Vector{ 0.0, 0.3 };
	lvl.getByName("C5-2")->vel = Vector{ -0.1, -0.2 };

	for (int i = 0; i < 100; i++)
		lvl.update(100ms);

	EXPECT_VEQ(lvl.getByName("A0-0")->rect, CollisionRect{ 2, 0, 1, 1 });
	EXPECT_VEQ(lvl.getByName("B0-1")->rect, CollisionRect{ 0, 4, 1, 1 });
	EXPECT_VEQ(lvl.getByName("C5-2")->rect, CollisionRect{ 4, 0, 1, 1 });


}

TEST(Level, ResolveCollision) {

	Level lvl(
		"A....."
		"B....."
		"......", 6, 3, &spriteFactory);

	lvl.getByName("A0-0")->vel = Vector{ 0.0, 0.0 };
	lvl.getByName("B0-1")->vel = Vector{ 0.0, -0.5 };

	lvl.update(500ms);

	EXPECT_VEQ(lvl.getByName("A0-0")->rect, CollisionRect{ 0, 0, 1, 1 });
	EXPECT_VEQ(lvl.getByName("B0-1")->rect, CollisionRect{ 0, 1, 1, 1 });
}

TEST(Level, DontGoThroughCorner) {

	Entity e(CollisionRect(1, 1, 1, 1), &sprite);
	e.vel = { 0,0 };

	Level lvl(
		"..|..."
		"..|..."
		"--+..."
		"......", 6, 4, &spriteFactory);

	lvl.addEntity(&e, "player");

	double speed = 15.0;
	e.vel.x = speed;

	e.vel.y = speed;

	for(int i=0;i<10;i++)
		lvl.update(50ms);

	EXPECT_NEAR(e.rect.x, 1, epsilon);
	EXPECT_NEAR(e.rect.y, 1, epsilon);



}