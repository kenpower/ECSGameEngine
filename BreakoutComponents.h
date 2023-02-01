#pragma once
#include<string>

struct LeftRightControlComponent : public Component {
	LeftRightControlComponent(double speed) :speed(speed) {}
	double speed;
};

struct BounceComponent : public Component {
};

struct DeleteAfterCollisionComponent : public Component {
};

struct ScoreWhenHitBlockComponent : public Component {
	int score;
	ScoreWhenHitBlockComponent(int score = 1 ):score(score) {}
};

struct PaddleBounceComponent : public Component {
	PaddleBounceComponent(double centre):paddleCentre(centre) {};
	double paddleCentre;
};