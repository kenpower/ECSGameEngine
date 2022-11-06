#pragma once
#include "../Entity.h"
#include "../Level.h"

using namespace std::chrono_literals;

class MockSprite : public Sprite {
public:
	virtual void draw(int, int);
};

class MockSpriteFactory : public SpriteFactory {
public:
	virtual Sprite* create(char);
};

const double epsilon = 0.0001;

void EXPECT_VEQ(Vector a, Vector b);

void EXPECT_VEQ(CollisionRect a, CollisionRect b);