#pragma once
#include "../Entity.h"
#include "../Level.h"

using namespace std::chrono_literals;

class MockSprite : public Sprite {
public:
	virtual void draw(int x, int y) {};
};


class MockSpriteFactory: public SpriteFactory {
public:
	virtual Sprite* create(char) { return new MockSprite(); };
};