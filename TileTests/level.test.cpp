#include "pch.h"

#include "../Level.h"

const double epsilon = 0.0001;

TEST(Level, CanGetRectByName) {

	Level lvl(
		"A....."
		"B....."
		".....C" , 6,3);

	EXPECT_NEAR(0, lvl.getByName("A").x, epsilon);
	EXPECT_NEAR(1, lvl.getByName("B").y, epsilon);
	EXPECT_NEAR(5, lvl.getByName("C").x, epsilon);

}
