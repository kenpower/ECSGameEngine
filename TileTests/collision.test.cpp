#include "pch.h"
#include "../Entity.h"
#include "../ECS.h"

void collisionSystem(Entities& entities);
double epsilon = 0.0001;

TEST(CollisionRect, Collisions) {
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

TEST(CollisionRect, CollidingWithTwoWalls) {
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
	entities.push_back(moving);

	collisionSystem(entities);

	pos = moving->getComponent<PositionComponent>();
	EXPECT_NEAR(pos->x, 0.1, epsilon);
	EXPECT_NEAR(pos->y, 1, epsilon);
	crc = moving->getComponent<CollisionResolvedComponent>();
	EXPECT_NEAR(crc->x, 0, epsilon);
	EXPECT_NEAR(crc->y, 1, epsilon);
	
}
