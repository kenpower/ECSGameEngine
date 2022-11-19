#include "pch.h"
#include "testhelpers.h"

//const double epsilon = 0.0001;


void EXPECT_VEQ(Vector a, Vector b) {

	EXPECT_NEAR(a.x, b.x, epsilon);
	EXPECT_NEAR(a.y, b.y, epsilon);
}

void EXPECT_VEQ(CollisionRect a, CollisionRect b) {

	EXPECT_NEAR(a.x, b.x, epsilon);
	EXPECT_NEAR(a.y, b.y, epsilon);
}