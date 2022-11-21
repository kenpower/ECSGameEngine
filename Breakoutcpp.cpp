#include <vector>
#include <chrono>
#include <memory>

#include "ConsoleRenderWindow.h"
#include "ECS.h"
#include "BreakoutSystems.h"

using namespace std;
using namespace std::chrono;


shared_ptr<Entity> wallEntity(const char* name, double x, double y, double w, double h) {	
	auto e = make_shared<Entity>(name);
	e->addComponent(make_shared<PositionComponent>(x, y));
	e->addComponent(make_shared<CollisionBoxComponent>(w, h));
	return e;
}

void game(ConsoleRenderWindow& crw, int worldWidth, int worldHeight) {

	Entities entities;

	auto unitBox = make_shared<CollisionBoxComponent>(1,1);

	auto ball = make_shared<Entity>("ball");
	ball->addComponent(make_shared<VelocityComponent>(10, 10));
	ball->addComponent(make_shared<PositionComponent>(10, 20));
	ball->addComponent(make_shared<CharSpriteComponent>('O'));
	ball->addComponent(make_shared<BounceComponent>());
	ball->addComponent(unitBox);
	entities.push_back(ball);

	auto paddle = make_shared<Entity>("paddle");
	paddle->addComponent(make_shared<PositionComponent>(10, 35));
	paddle->addComponent(make_shared<StringSpriteComponent>("<====>"));
	paddle->addComponent(make_shared<CollisionBoxComponent>(6,1));
	paddle->addComponent(make_shared<LeftRightControlComponent>(15));
	entities.push_back(paddle);

	entities.push_back(wallEntity("", -1, -1, 1, worldHeight+2));
	entities.push_back(wallEntity("", worldWidth, -1, 1, worldHeight+2));
	entities.push_back(wallEntity("", -1, 0, worldWidth+2, 1)); //leave top line clear for dev info
	entities.push_back(wallEntity("", -1, worldHeight,  worldWidth + 2, worldHeight+1));


	auto block1 = make_shared<StringSpriteComponent>("XXX");
	auto block2 = make_shared<StringSpriteComponent>("###");
	auto scoreWhenHit = make_shared<ScoreWhenHitComponent>(5);

	auto blockCollision = make_shared<CollisionBoxComponent>(3, 1);
	auto block = make_shared<BlockComponent>();

	int firstBlock = 0;
	for (int x = 3; x < worldWidth - 4; x += 3) {
		int blockColor = firstBlock % 2;
		for (int y = 5; y < 20; y += 1) {
			auto e = make_shared<Entity>("block");
			e->addComponent(make_shared<PositionComponent>(x, y));
			e->addComponent(blockColor % 2 ? block1 : block2);
			e->addComponent(blockCollision);
			e->addComponent(block);
			e->addComponent(scoreWhenHit);

			entities.push_back(e);
			blockColor++;
		}
		firstBlock++;
	}

	bool bGameOver = false;

	long frameCounter = 0;

	auto startOfFrame = std::chrono::high_resolution_clock::now();
	auto startFrameTimer = std::chrono::high_resolution_clock::now();

	std::wstring wFPSstring;

	int gameScore = 0;

	while (!bGameOver) {
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> secondsSinceLastFrame = now - startOfFrame;
		startOfFrame = now;

		movementSystem(entities, secondsSinceLastFrame.count());

		userControlSystem(entities, secondsSinceLastFrame.count());

		collisionSystem(entities);

		bounceSystem(entities);

		scoreBlocksSystem(entities, gameScore);
		
		deadBlocksSystem(entities);

		renderCharOnConsoleSystem(entities, crw);

		renderStringOnConsoleSystem(entities, crw);


		//remove temporary components
		for (auto& e : entities) {
			e->removeComponent(CollisionResolvedComponent::NAME);
			e->removeComponent(MovedComponent::NAME);
			e->removeComponent(CollidedComponent::NAME);
		}

		static int framesPerSec = 0;
		int frameCountWindow = 100;
		if (frameCounter++ % frameCountWindow == 0) {
			auto now = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed_time = now - startFrameTimer;

			startFrameTimer = std::chrono::high_resolution_clock::now();
		
			framesPerSec = (int)(frameCountWindow / elapsed_time.count());
		
		}

		const std::string s = "FPS:" + to_string(framesPerSec)+ "  Score:" + to_string(gameScore);
		wFPSstring = std::wstring(s.begin(), s.end());

		crw.DrawString(0, 0, wFPSstring.c_str());
		crw.Show();
		crw.Clear();
	}

}