#include "pch.h"
#include "testhelpers.h"

//const double epsilon = 0.0001;


void EXPECT_VEQ(Vector a, Vector b) {

	EXPECT_NEAR(a.x, b.x, epsilon) << "ax=" << a.x << ", bx=" << b.x;
	EXPECT_NEAR(a.y, b.y, epsilon) << "ay=" << a.x << ", by=" << b.x;;
}
