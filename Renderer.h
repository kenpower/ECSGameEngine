#pragma once

class Sprite {
public:
	virtual void draw(double x, double y, double rotate=0) = 0;
};

class ConsoleSprite : public Sprite {
public:

	void render(double x, double y, double rotate = 0) {

	}
};