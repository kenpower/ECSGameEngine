#include "pch.h"
#include "vector"
#include <utility>
#include "testhelpers.h"
#include "../ECS.h"
#include "../vector.h"
using namespace std;




void collisionSystem(Components&);
Vector getMinimumTranslationVector(const CollisionRectComponent*, const CollisionRectComponent*);

TEST(MTV, NoOverlap) {
	Vector zero{ 0,0 };
	vector<pair<CollisionRectComponent, Vector>> td = {
		{CollisionRectComponent{ -2, 1, 1, 1 }, zero},
		{CollisionRectComponent{ -1, 1, 1, 1 }, zero},
		{CollisionRectComponent{ -0.5, 1, 1, 1 }, zero},
		{CollisionRectComponent{ 0, 1, 1, 1 }, zero},
		{CollisionRectComponent{ 0.25, 1, 1, 1 }, zero},
		{CollisionRectComponent{ 1, 1, 1, 1 }, zero},
		{CollisionRectComponent{ 2, 1, 1, 1 }, zero},

		{CollisionRectComponent{ -2, -1, 1, 1 }, zero},
		{CollisionRectComponent{ -1, -1, 1, 1 }, zero},
		{CollisionRectComponent{ -0.5, -1, 1, 1 }, zero},
		{CollisionRectComponent{ 0, -1, 1, 1 }, zero},
		{CollisionRectComponent{ 0.25, -1, 1, 1 }, zero},
		{CollisionRectComponent{ 1, -1, 1, 1 }, zero},
		{CollisionRectComponent{ 2, -1, 1, 1 }, zero},

		{CollisionRectComponent{ -1, 0, 1, 1 }, zero},
		{CollisionRectComponent{ 1, 0, 1, 1 }, zero},
		{CollisionRectComponent{ -2, 0, 1, 1 }, zero},
		{CollisionRectComponent{ 2, 0, 1, 1 }, zero},
	};
	CollisionRectComponent cr{ 0, 0, 1, 1 };
	
	for (const auto& t:td)
			EXPECT_VEQ(t.second, getMinimumTranslationVector(&t.first, &cr));
}

TEST(MTV, Overlap) {
	Vector zero{ 0,0 };
	vector<pair<CollisionRectComponent, Vector>> td = {
		{CollisionRectComponent{ -0.9, 0, 1, 1 }, Vector{-0.1,0}},
		{CollisionRectComponent{ -0.6, 0, 1, 1 },  Vector{-0.4,0}},
		{CollisionRectComponent{ +0.6, 0, 1, 1 },  Vector{0.4,0}},
		{CollisionRectComponent{ +0.9, 0, 1, 1 },  Vector{0.1,0}},

		{CollisionRectComponent{ -0.9, -0.6, 1, 1 }, Vector{-0.1,0}},
		{CollisionRectComponent{ -0.5, -0.6, 1, 1 },  Vector{0,-0.4}},
		{CollisionRectComponent{ +0.5, -0.6, 1, 1 },  Vector{0,-0.4}},
		{CollisionRectComponent{ +0.9, -0.6, 1, 1 },  Vector{0.1,0}},

		{CollisionRectComponent{ -0.9, 0.6, 1, 1 }, Vector{-0.1,0}},
		{CollisionRectComponent{ -0.5, 0.6, 1, 1 },  Vector{0,+0.4}},
		{CollisionRectComponent{ +0.5, 0.6, 1, 1 },  Vector{0,+0.4}},
		{CollisionRectComponent{ +0.9, 0.6, 1, 1 },  Vector{0.1,0}},
	};
	CollisionRectComponent cr{ 0, 0, 1, 1 };

	for (auto& t : td)
		EXPECT_VEQ(t.second, getMinimumTranslationVector(&t.first, &cr)) ;
}

TEST(MTV, FirstIsSmaller) {
	Vector zero{ 0,0 };
	vector<pair<CollisionRectComponent, Vector>> td = {
		{CollisionRectComponent{ 1, 0, 1, 1 }, Vector{0,-1}},
		{CollisionRectComponent{ 4, 2, 1, 1 },  Vector{0,-3}},
		{CollisionRectComponent{ 4, 4.5, 1, 1 },  Vector{-5,0}},
		{CollisionRectComponent{ 6, 4.5, 1, 1 },  Vector{4,0}},
		{CollisionRectComponent{ 6, 8, 1, 1 },  Vector{0,2}},

		{CollisionRectComponent{ -0.5, -0.9, 1, 1 }, Vector{0, -0.1}},
		{CollisionRectComponent{ -0.5, 9.9, 1, 1 },  Vector{0, +0.1}},
		{CollisionRectComponent{ -0.5, 0.1, 1, 1 }, Vector{-0.5, -0}},
		{CollisionRectComponent{ -0.5, 9.1, 1, 1 },  Vector{-0.5, 0}},
		{CollisionRectComponent{  9.5, -0.9, 1, 1 }, Vector{0, -0.1}},
		{CollisionRectComponent{  9.5, 9.9, 1, 1 },  Vector{0, +0.1}},
		{CollisionRectComponent{  9.5, 0.1, 1, 1 }, Vector{+0.5, -0}},
		{CollisionRectComponent{  9.5, 9.1, 1, 1 },  Vector{+0.5, 0}},
	};
	CollisionRectComponent cr{ 0, 0, 10, 10 };

	for (const auto& t : td)
		EXPECT_VEQ(t.second, getMinimumTranslationVector(&t.first, &cr));
}

TEST(MTV, FirstIsBigger) {
	Vector zero{ 0,0 };
	vector<pair<CollisionRectComponent, Vector>> td = {
		{CollisionRectComponent{ 0, -2, 10, 10 }, Vector{1,0}},
		{CollisionRectComponent{ -8, -9, 10, 10 }, Vector{0,-1}},
		{CollisionRectComponent{ -0.5, -0.1, 10, 10 }, Vector{0, +1.1}},
		{CollisionRectComponent{ -0.5, -0.9, 10, 10 }, Vector{1.5, 0}},
	};
	CollisionRectComponent cr{ 0, 0, 1, 1 };

	for (const auto& t : td)
		EXPECT_VEQ(t.second, getMinimumTranslationVector(&t.first, &cr));
}

TEST(CollisionSystem, Collisions) {
	Components components;

	EntityID stationary = 0;
	components.positions[stationary] = new PositionComponent(0, 0);
	components.collisionRects[stationary] = new CollisionRectComponent(1, 1);


	EntityID moving = 1;
	components.positions[moving] = new PositionComponent(0, 0.1);
	components.collisionRects[moving] = new CollisionRectComponent(1, 1);
	components.moveds[moving] = new MovedComponent;

	collisionSystem(components);
	auto pos = components.positions[moving];
	EXPECT_NEAR(pos->x, 0, epsilon);
	EXPECT_NEAR(pos->y, 1, epsilon);
	auto crc = components.collideds[moving];
	EXPECT_VEQ(crc->surfaceNormal, Vector{0,1});


	pos = components.positions[stationary];
	EXPECT_NEAR(pos->x, 0, epsilon);
	EXPECT_NEAR(pos->y, 0, epsilon);

	components.positions[moving] = new PositionComponent(0.9, 0.0);
	components.collideds.erase(moving);
	components.collideds.erase(stationary);

	collisionSystem(components);
	pos = components.positions[moving];
	EXPECT_NEAR(pos->x, 1, epsilon);
	EXPECT_NEAR(pos->y, 0, epsilon);
	crc = components.collideds[moving];
	EXPECT_VEQ(crc->surfaceNormal, Vector{ 1,0 });

	components.positions[moving] = new PositionComponent(0.0, -0.9);
	components.collideds.erase(moving);
	components.collideds.erase(stationary);

	collisionSystem(components);
	pos = components.positions[moving];
	EXPECT_NEAR(pos->x, 0, epsilon);
	EXPECT_NEAR(pos->y, -1, epsilon);
	crc = components.collideds[moving];
	EXPECT_VEQ(crc->surfaceNormal, Vector{ 0, -1 });

	components.positions[moving] = new PositionComponent(-0.9, 0.0);
	components.collideds.erase(moving);
	components.collideds.erase(stationary);

	collisionSystem(components);
	pos = components.positions[moving];
	EXPECT_NEAR(pos->x, -1, epsilon);
	EXPECT_NEAR(pos->y, 0, epsilon);
	crc = components.collideds[moving];
	EXPECT_VEQ(crc->surfaceNormal, Vector{ -1, 0 });

}

TEST(CollisionSystem, CollidingWithTwoWalls) {
	Components components;

	EntityID stationary1 = 0;
	components.positions[stationary1] = new PositionComponent(0, 0);
	components.collisionRects[stationary1] = new CollisionRectComponent(1, 1);
	EntityID stationary2 = 1;
	components.positions[stationary2] = new PositionComponent(1, 0);
	components.collisionRects[stationary2] = new CollisionRectComponent(1, 1);

	EntityID moving = 2;
	components.positions[moving] = new PositionComponent(0.2, 0.9);
	components.collisionRects[moving] = new CollisionRectComponent(1, 1);
	components.moveds[moving] = new MovedComponent;

	collisionSystem(components);

	auto pos = components.positions[moving];
	EXPECT_TRUE((pos->x == 1 || pos->y == 1));
	auto crc = components.collideds[moving];
	EXPECT_VEQ(crc->surfaceNormal, Vector{ 0,1 });

	components.positions[moving] = new PositionComponent(0.9, 0.8);

	collisionSystem(components);

	pos = components.positions[moving];
	EXPECT_TRUE((pos->x == 1 || pos->y == 1));
	crc = components.collideds[moving];
	EXPECT_VEQ(crc->surfaceNormal, Vector{ 0, 1 });

	components.positions[moving] = new PositionComponent(-0.9, 0.8);

	collisionSystem(components);

	pos = components.positions[moving];
	EXPECT_TRUE((pos->x == -1 || pos->y == -1));
	crc = components.collideds[moving];
	EXPECT_VEQ(crc->surfaceNormal, Vector{ -1, 0 });

}

TEST(Vector, Equality) {
	Vector v{ 1, 0 };

	EXPECT_TRUE((v == Vector{ 1,0 }));
}

TEST(CollisionSystem, CollidingWithCorner) {

	Components components;

	EntityID stationary1 = 1;
	components.positions[stationary1] = new PositionComponent(0, 0);
	components.collisionRects[stationary1] = new CollisionRectComponent(1, 1);
	EntityID stationary2 = 2;
	components.positions[stationary2] = new PositionComponent(1, 0);
	components.collisionRects[stationary2] = new CollisionRectComponent(1, 1);
	EntityID stationary3 = 3;
	components.positions[stationary3] = new PositionComponent(-1, 0);
	components.collisionRects[stationary3] = new CollisionRectComponent(1, 1);
	EntityID stationary4 = 4;
	components.positions[stationary4] = new PositionComponent(0, -1);
	components.collisionRects[stationary4] = new CollisionRectComponent(1, 1);
	EntityID stationary5 = 5;
	components.positions[stationary5] = new PositionComponent(0,1);
	components.collisionRects[stationary5] = new CollisionRectComponent(1, 1);

	EntityID moving = 0;
	components.positions[moving] = new PositionComponent(-0.8, -0.9);
	components.collisionRects[moving] = new CollisionRectComponent(1, 1);
	components.moveds[moving] = new MovedComponent;

	collisionSystem(components);
	
	auto pos = components.positions[moving];
	EXPECT_NEAR(pos->x, -1, epsilon);
	EXPECT_NEAR(pos->y, -1, epsilon);
	auto crc = components.collideds[moving];
	EXPECT_TRUE(
		(crc->surfaceNormal == Vector{-1,  0 } || crc->surfaceNormal == Vector{ 0,  -1 })
	); // upper left quadrant


	components.positions[moving] = new PositionComponent(0.8, 0.8);

	collisionSystem(components);

	pos = components.positions[moving];
	EXPECT_NEAR(pos->x, 1, epsilon);
	EXPECT_NEAR(pos->y, 1, epsilon);
	crc = components.collideds[moving];
	EXPECT_TRUE(
		(crc->surfaceNormal == Vector{ 1,  0 } || crc->surfaceNormal == Vector{ 0,  -1 })
	); // upper right quadrant



	components.positions[moving] = new PositionComponent(-0.8, 0.8);

	collisionSystem(components);

	pos = components.positions[moving];
	EXPECT_NEAR(pos->x, -1, epsilon);
	EXPECT_NEAR(pos->y, 1, epsilon);
	crc = components.collideds[moving];
	EXPECT_TRUE(
		(crc->surfaceNormal == Vector{ -1,  0 } || crc->surfaceNormal == Vector{ 0,  1 })
	); // upper right quadrant

	
	components.positions[moving] = new PositionComponent(0.8, -0.8);

	collisionSystem(components);

	pos = components.positions[moving];
	EXPECT_NEAR(pos->x, 1, epsilon);
	EXPECT_NEAR(pos->y, -1, epsilon);
	crc = components.collideds[moving];
	EXPECT_TRUE(
		(crc->surfaceNormal == Vector{ 1,  0 } || crc->surfaceNormal == Vector{ 0,  -1 })
	); // upper right quadrant

	
}
