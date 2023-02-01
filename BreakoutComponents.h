#pragma once
#include<string>

struct LeftRightControlComponent {
	double speed;
};

struct BounceComponent : public Component {
};

struct DeleteAfterCollisionComponent {
};

struct ScoreWhenHitBlockComponent{
	int score;
};

struct PaddleBounceComponent  {
	double paddleCentre;
};