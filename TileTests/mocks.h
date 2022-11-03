#pragma once
#include"../Entity.h"
#include "../Level.h"

class MockSprite : public Sprite {
public:
	virtual void draw(int x, int y) {};
};

using namespace std::chrono_literals;

class MockSpriteFactory: public SpriteFactory {
public:
	virtual Sprite* create(char) { return new MockSprite(); };
};