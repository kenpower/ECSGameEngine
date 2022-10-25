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

	resolveCollision(a, b);
	EXPECT_FALSE(isColliding(a, b)) << a.toString() << " " << b.toString();

	resolveCollision(e, b);
	EXPECT_FALSE(isColliding(a, b)) << a.toString() << " " << b.toString();

	resolveCollision(f, b);
	EXPECT_FALSE(isColliding(a, b)) << a.toString() << " " << b.toString();

	resolveCollision(b1, b);
	EXPECT_FALSE(isColliding(a, b)) << a.toString() << " " << b.toString();

}