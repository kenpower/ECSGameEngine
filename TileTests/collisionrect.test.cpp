#include "pch.h"

#include "../CollisionRect.h"

TEST(CollisionRect, Collisions) {
	CollisionRect a(0, 0, 10, 10);
	CollisionRect b(5, 5, 10, 10);
	CollisionRect c(20, 20, 10, 10);
	CollisionRect d(10, 10, 10, 10);
	CollisionRect e(10, 0, 10, 10);


	EXPECT_TRUE(isColliding(a, b));
	EXPECT_FALSE(isColliding(a, c));
	EXPECT_TRUE(isColliding(a, a));
	EXPECT_FALSE(isColliding(a, d));
	EXPECT_FALSE(isColliding(a, e));
}

TEST(CollisionRect, CollisionResolution) {
	CollisionRect a(0, 0, 10, 10);
	CollisionRect b(5, 5, 10, 10);
	CollisionRect b1(5, 5, 10, 10);
	CollisionRect c(20, 20, 10, 10);
	CollisionRect d(10, 10, 10, 10);
	CollisionRect e(10, 0, 10, 10);
	CollisionRect f(5, 10, 10, 10);

	Vector adjustment{ 0,0 };
	_resolveCollision(a, b, adjustment);
	EXPECT_FALSE(isColliding(a, b)) << a.toString() << " " << b.toString();

	_resolveCollision(e, b, adjustment);
	EXPECT_FALSE(isColliding(a, b)) << a.toString() << " " << b.toString();

	_resolveCollision(f, b, adjustment);
	EXPECT_FALSE(isColliding(a, b)) << a.toString() << " " << b.toString();

	_resolveCollision(b1, b, adjustment);
	EXPECT_FALSE(isColliding(a, b)) << a.toString() << " " << b.toString();

}

TEST(CollisionRect, CollisionResolutionFractional) {
	CollisionRect a(0, 0, 1, 1);
	CollisionRect b(0, 0.75, 1, 1);

	Vector adjustment{ 0,0 };
	_resolveCollision(b, a, adjustment);
	EXPECT_FALSE(isColliding(a, b)) << a.toString() << " " << b.toString();

	EXPECT_EQ(1, b.y) << a.toString() << " " << b.toString();

}