#include "pch.h"
#include "vector"
#include <utility>
#include "testhelpers.h"
#include "../Entity.h"
#include "../ECS.h"
#include "../vector.h"
using namespace std;


struct CollisionRect
{
	double x;
	double y;
	double w;
	double h;
};

void collisionSystem(Entities&);
Vector getMinimumTranslationVector(const CollisionRect&, const CollisionRect&);

TEST(MTV, NoOverlap) {
	Vector zero{ 0,0 };
	vector<pair<CollisionRect, Vector>> td = {
		{CollisionRect{ -2, 1, 1, 1 }, zero},
		{CollisionRect{ -1, 1, 1, 1 }, zero},
		{CollisionRect{ -0.5, 1, 1, 1 }, zero},
		{CollisionRect{ 0, 1, 1, 1 }, zero},
		{CollisionRect{ 0.25, 1, 1, 1 }, zero},
		{CollisionRect{ 1, 1, 1, 1 }, zero},
		{CollisionRect{ 2, 1, 1, 1 }, zero},

		{CollisionRect{ -2, -1, 1, 1 }, zero},
		{CollisionRect{ -1, -1, 1, 1 }, zero},
		{CollisionRect{ -0.5, -1, 1, 1 }, zero},
		{CollisionRect{ 0, -1, 1, 1 }, zero},
		{CollisionRect{ 0.25, -1, 1, 1 }, zero},
		{CollisionRect{ 1, -1, 1, 1 }, zero},
		{CollisionRect{ 2, -1, 1, 1 }, zero},

		{CollisionRect{ -1, 0, 1, 1 }, zero},
		{CollisionRect{ 1, 0, 1, 1 }, zero},
		{CollisionRect{ -2, 0, 1, 1 }, zero},
		{CollisionRect{ 2, 0, 1, 1 }, zero},
	};
	CollisionRect cr{ 0, 0, 1, 1 };
	
	for (const auto& t:td)
			EXPECT_VEQ(t.second, getMinimumTranslationVector(t.first, cr));
}

TEST(MTV, Overlap) {
	Vector zero{ 0,0 };
	vector<pair<CollisionRect, Vector>> td = {
		{CollisionRect{ -0.9, 0, 1, 1 }, Vector{-0.1,0}},
		{CollisionRect{ -0.6, 0, 1, 1 },  Vector{-0.4,0}},
		{CollisionRect{ +0.6, 0, 1, 1 },  Vector{0.4,0}},
		{CollisionRect{ +0.9, 0, 1, 1 },  Vector{0.1,0}},

		{CollisionRect{ -0.9, -0.6, 1, 1 }, Vector{-0.1,0}},
		{CollisionRect{ -0.5, -0.6, 1, 1 },  Vector{0,-0.4}},
		{CollisionRect{ +0.5, -0.6, 1, 1 },  Vector{0,-0.4}},
		{CollisionRect{ +0.9, -0.6, 1, 1 },  Vector{0.1,0}},
	
		{CollisionRect{ -0.9, 0.6, 1, 1 }, Vector{-0.1,0}},
		{CollisionRect{ -0.5, 0.6, 1, 1 },  Vector{0,+0.4}},
		{CollisionRect{ +0.5, 0.6, 1, 1 },  Vector{0,+0.4}},
		{CollisionRect{ +0.9, 0.6, 1, 1 },  Vector{0.1,0}},
	};
	CollisionRect cr{ 0, 0, 1, 1 };

	for (auto& t : td)
		EXPECT_VEQ(t.second, getMinimumTranslationVector(t.first, cr));
}

TEST(MTV, FirstIsSmaller) {
	Vector zero{ 0,0 };
	vector<pair<CollisionRect, Vector>> td = {
		{CollisionRect{ 1, 0, 1, 1 }, Vector{0,-1}},
		{CollisionRect{ 4, 2, 1, 1 },  Vector{0,-3}},
		{CollisionRect{ 4, 4.5, 1, 1 },  Vector{-5,0}},
		{CollisionRect{ 6, 4.5, 1, 1 },  Vector{4,0}},
		{CollisionRect{ 6, 8, 1, 1 },  Vector{0,2}},

		{CollisionRect{ -0.5, -0.9, 1, 1 }, Vector{0, -0.1}},
		{CollisionRect{ -0.5, 9.9, 1, 1 },  Vector{0, +0.1}},
		{CollisionRect{ -0.5, 0.1, 1, 1 }, Vector{-0.5, -0}},
		{CollisionRect{ -0.5, 9.1, 1, 1 },  Vector{-0.5, 0}},
		{CollisionRect{  9.5, -0.9, 1, 1 }, Vector{0, -0.1}},
		{CollisionRect{  9.5, 9.9, 1, 1 },  Vector{0, +0.1}},
		{CollisionRect{  9.5, 0.1, 1, 1 }, Vector{+0.5, -0}},
		{CollisionRect{  9.5, 9.1, 1, 1 },  Vector{+0.5, 0}},
	};
	CollisionRect cr{ 0, 0, 10, 10 };

	for (const auto& t : td)
		EXPECT_VEQ(t.second, getMinimumTranslationVector(t.first, cr));
}

TEST(MTV, FirstIsBigger) {
	Vector zero{ 0,0 };
	vector<pair<CollisionRect, Vector>> td = {
		{CollisionRect{ 0, -2, 10, 10 }, Vector{1,0}},		
		{CollisionRect{ -8, -9, 10, 10 }, Vector{0,-1}},
		{CollisionRect{ -0.5, -0.1, 10, 10 }, Vector{0, +1.1}},
		{CollisionRect{ -0.5, -0.9, 10, 10 }, Vector{1.5, 0}},
	};
	CollisionRect cr{ 0, 0, 1, 1 };

	for (const auto& t : td)
		EXPECT_VEQ(t.second, getMinimumTranslationVector(t.first, cr));
}

TEST(CollisionSystem, Collisions) {
	Entities entities;

	auto stationary = make_shared<Entity>("stationary");
	stationary->addComponent(make_shared<PositionComponent>(0, 0));
	stationary->addComponent(make_shared<CollisionBoxComponent>(1, 1));
	entities.push_back(stationary);

	auto moving = make_shared<Entity>("moving");
	moving->addComponent(make_shared<PositionComponent>(0, 0.9));
	moving->addComponent(make_shared<CollisionBoxComponent>(1, 1));
	moving->addComponent(make_shared<MovedComponent>());
	entities.push_back(moving);
	collisionSystem(entities);

	auto pos = moving->getComponent<PositionComponent>();
	EXPECT_NEAR(pos->x, 0, epsilon);
	EXPECT_NEAR(pos->y, 1, epsilon);
	auto crc = moving->getComponent<CollisionResolvedComponent>();
	EXPECT_NEAR(crc->x, 0, epsilon);
	EXPECT_NEAR(crc->y, 1, epsilon);

	pos = stationary->getComponent<PositionComponent>();
	EXPECT_NEAR(pos->x, 0, epsilon);
	EXPECT_NEAR(pos->y, 0, epsilon);

	
	moving->addComponent(make_shared<PositionComponent>(0.9, 0));
	moving->removeComponent(CollisionResolvedComponent::NAME);
	
	collisionSystem(entities);

	pos = moving->getComponent<PositionComponent>();
	EXPECT_NEAR(pos->x, 1, epsilon);
	EXPECT_NEAR(pos->y, 0, epsilon);
	crc = moving->getComponent<CollisionResolvedComponent>();
	EXPECT_NEAR(crc->x, 1, epsilon);
	EXPECT_NEAR(crc->y, 0, epsilon);

	moving->addComponent(make_shared<PositionComponent>(0, -0.9));
	moving->removeComponent(CollisionResolvedComponent::NAME);

	collisionSystem(entities);

	pos = moving->getComponent<PositionComponent>();
	EXPECT_NEAR(pos->x, 0, epsilon);
	EXPECT_NEAR(pos->y, -1, epsilon);
	crc = moving->getComponent<CollisionResolvedComponent>();
	EXPECT_NEAR(crc->x, 0, epsilon);
	EXPECT_NEAR(crc->y, -1, epsilon);

	moving->addComponent(make_shared<PositionComponent>(-0.9, 0));
	moving->removeComponent(CollisionResolvedComponent::NAME);

	collisionSystem(entities);

	pos = moving->getComponent<PositionComponent>();
	EXPECT_NEAR(pos->x, -1, epsilon);
	EXPECT_NEAR(pos->y, 0, epsilon);
	crc = moving->getComponent<CollisionResolvedComponent>();
	EXPECT_NEAR(crc->x, -1, epsilon);
	EXPECT_NEAR(crc->y, 0, epsilon);
}

TEST(CollisionSystem, CollidingWithTwoWalls) {
	Entities entities;

	auto stationary1 = make_shared<Entity>("stationary");
	stationary1->addComponent(make_shared<PositionComponent>(0, 0));
	stationary1->addComponent(make_shared<CollisionBoxComponent>(1, 1));
	entities.push_back(stationary1);

	auto stationary2 = make_shared<Entity>("stationary");
	stationary2->addComponent(make_shared<PositionComponent>(1, 0));
	stationary2->addComponent(make_shared<CollisionBoxComponent>(1, 1));
	entities.push_back(stationary2);

	auto moving = make_shared<Entity>("moving");
	moving->addComponent(make_shared<PositionComponent>(0.2, 0.9));
	moving->addComponent(make_shared<CollisionBoxComponent>(1, 1));
	moving->addComponent(make_shared<MovedComponent>());
	entities.push_back(moving);

	collisionSystem(entities);

	auto pos = moving->getComponent<PositionComponent>();
	EXPECT_NEAR(pos->x, 0.2, epsilon);
	EXPECT_NEAR(pos->y, 1, epsilon);
	auto crc = moving->getComponent<CollisionResolvedComponent>();
	EXPECT_NEAR(crc->x, 0, epsilon);
	EXPECT_NEAR(crc->y, 1, epsilon);

	moving->addComponent(make_shared<PositionComponent>(0.1, 0.8));

	collisionSystem(entities);

	pos = moving->getComponent<PositionComponent>();
	EXPECT_NEAR(pos->x, 0.1, epsilon);
	EXPECT_NEAR(pos->y, 1, epsilon);
	crc = moving->getComponent<CollisionResolvedComponent>();
	EXPECT_NEAR(crc->x, 0, epsilon);
	EXPECT_NEAR(crc->y, 1, epsilon);
	
}

TEST(CollisionSystem, CollidingWithCorner) {
	Entities entities;

	auto stationary1 = make_shared<Entity>("stationary");
	stationary1->addComponent(make_shared<PositionComponent>(0, 0));
	stationary1->addComponent(make_shared<CollisionBoxComponent>(1, 1));
	entities.push_back(stationary1);

	auto stationary2 = make_shared<Entity>("stationary");
	stationary2->addComponent(make_shared<PositionComponent>(1, 0));
	stationary2->addComponent(make_shared<CollisionBoxComponent>(1, 1));
	entities.push_back(stationary2);

	auto stationary3 = make_shared<Entity>("stationary");
	stationary3->addComponent(make_shared<PositionComponent>(-1, 0));
	stationary3->addComponent(make_shared<CollisionBoxComponent>(1, 1));
	entities.push_back(stationary3);

	auto stationary4 = make_shared<Entity>("stationary");
	stationary4->addComponent(make_shared<PositionComponent>(0, -1));
	stationary4->addComponent(make_shared<CollisionBoxComponent>(1, 1));
	entities.push_back(stationary4);

	auto stationary5 = make_shared<Entity>("stationary");
	stationary5->addComponent(make_shared<PositionComponent>(0, 1));
	stationary5->addComponent(make_shared<CollisionBoxComponent>(1, 1));
	entities.push_back(stationary5);


	auto moving = make_shared<Entity>("moving");
	moving->addComponent(make_shared<CollisionBoxComponent>(1, 1));
	moving->addComponent(make_shared<MovedComponent>());
	entities.push_back(moving);

	moving->addComponent(make_shared<PositionComponent>(-0.8, -0.9));
	collisionSystem(entities);

	auto pos = moving->getComponent<PositionComponent>();
	EXPECT_NEAR(pos->x, -1, epsilon);
	EXPECT_NEAR(pos->y, -1, epsilon);
	auto crc = moving->getComponent<CollisionResolvedComponent>();
	EXPECT_NEAR(crc->x + crc->y, -1, epsilon); // upper left quadrant

	moving->addComponent(make_shared<PositionComponent>(0.8, 0.8));

	collisionSystem(entities);

	pos = moving->getComponent<PositionComponent>();
	EXPECT_NEAR(pos->x, 1, epsilon);
	EXPECT_NEAR(pos->y, 1, epsilon);
	crc = moving->getComponent<CollisionResolvedComponent>();
	EXPECT_NEAR(crc->x + crc->y, 1, epsilon); // bottom right quadrant

	moving->addComponent(make_shared<PositionComponent>(-0.8, 0.8));

	collisionSystem(entities);

	pos = moving->getComponent<PositionComponent>();
	EXPECT_NEAR(pos->x, -1, epsilon);
	EXPECT_NEAR(pos->y, 1, epsilon);
	crc = moving->getComponent<CollisionResolvedComponent>();

	moving->addComponent(make_shared<PositionComponent>(0.8, -0.8));

	collisionSystem(entities);

	pos = moving->getComponent<PositionComponent>();
	EXPECT_NEAR(pos->x, 1, epsilon);
	EXPECT_NEAR(pos->y, -1, epsilon);
	crc = moving->getComponent<CollisionResolvedComponent>();


}
