#include "pch.h"

#include "../Level.h"

const double epsilon = 0.0001;

TEST(Level, CanGetEntityByName) {

	Level lvl(
		"A....."
		"B....."
		".....C" , 6,3);

	EXPECT_NEAR(0, lvl.getByName("A").rect.x, epsilon);
	EXPECT_NEAR(1, lvl.getByName("B").rect.y, epsilon);
	EXPECT_NEAR(5, lvl.getByName("C").rect.x, epsilon);

}
