#include "pch.h"

#include "../ball.h"

#include "testhelpers.h"



TEST(Ball, DirectionChangeAfterColllision) {

	//Level level(
	//	"....E."
	//	"ABCDF."
	//	"....G."
	//	"....H.", 6, 6, &spriteFactory);

	//Ball ball(CollisionRect(0, 1, 1, 1), &sprite);
	//
	//level.addEntity(&ball, "ball");
	//double startingPositions[][6]{
	//	{1.0,2.0,1,-1,1,1},
	//	{1.0,1.9,1,-1,1,1},
	//	{0.9,1.9,1,-1,1,1},
	//	{0.8,1.9,1,-1,1,1},
	//	{0.9,1.8,1,-1,1,1},

	//	{0.0,0.0,1,1,1,-1},
	//	{0.0,0.9,1,1,1,-1},
	//	{0.9,0.9,1,1,1,-1},
	//	{0.8,0.9,1,1,1,-1},
	//	{0.9,0.8,1,1,1,-1},

	//	{3.0,3.0,1,1,-1,1},
	//	{3.0,3.1,1,1,-1,1},
	//	{3.1,3.1,1,1,-1,1},
	//	{3.2,3.1,1,1,-1,1},
	//	{3.1,3.2,1,1,-1,1},


	//	{5.0,1.0,-1,1,1,1},
	//	{5.0,1.1,-1,1,1,1},
	//	{4.9,1.1,-1,1,1,1},
	//	{4.8,1.1,-1,1,1,1},
	//	{4.9,1.2,-1,1,1,1},

	//};

	//for (auto t : startingPositions) {
	//	ball.rect = CollisionRect{ t[0], t[1],1,1};
	//	ball.vel = Vector{ t[2],t[3]};
	//	level.update(10ms);
	//	EXPECT_NEAR(t[4], ball.vel.x, epsilon);
	//	EXPECT_NEAR(t[5], ball.vel.y, epsilon);
	//}

}