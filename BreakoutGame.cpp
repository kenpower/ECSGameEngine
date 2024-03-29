#include <vector>
#include <chrono>
#include <memory>

#include "./ECSGameEngine/ECS.h"
#include "ECSSystems.h"
#include "BreakoutSystems.h"

using namespace std;
using namespace std::chrono;

class ConsoleRenderWindow;

void wallEntity(Components& components, int x, int y, int w, int h) {	
	auto wallID = newEntityID();
	components.positions[wallID] = new PositionComponent{ (double)x, (double)y };
	components.collisionRects[wallID] = new CollisionRectComponent(w, h);
}

void breakoutGame(ConsoleRenderWindow& crw, int worldWidth, int worldHeight) {

	Components components;

	auto unitBox = make_shared<CollisionRectComponent>(1, 1);

	auto ballID = newEntityID();
	components.velocitys[ballID] = new VelocityComponent{ 10., 10. };
	components.positions[ballID] = new PositionComponent{ 10., 20. };
	components.charSprites[ballID] = new CharSpriteComponent{ 'O' };
	components.bounces[ballID] = new BounceComponent();
	components.collisionRects[ballID] = new CollisionRectComponent(1, 1);


	auto paddleID = newEntityID();
	components.positions[paddleID] = new PositionComponent{ 10., 35. };
	components.stringSprites[paddleID] = new StringSpriteComponent{"<====>"};
	components.collisionRects[paddleID] = new CollisionRectComponent(6, 1);
	components.leftRightControls[paddleID] = new LeftRightControlComponent{ 15. };
	components.paddleBounces[paddleID] = new PaddleBounceComponent{ 3. };


	wallEntity(components, -1, -1, 1, worldHeight+2);
	wallEntity(components, worldWidth, -1, 1, worldHeight+2);
	wallEntity(components, -1, 0, worldWidth+2, 1); //leave top line clear for dev info
	wallEntity(components, -1, worldHeight,  worldWidth + 2, worldHeight+1);

	int firstBlock = 0;
	for (int x = 3; x < worldWidth - 4; x += 3) {
		int blockColor = firstBlock % 2;
		for (int y = 5; y < 20; y += 1) {
			auto blockID = newEntityID();
			components.positions[blockID] = new PositionComponent{ (double)x, (double)y };
			components.stringSprites[blockID] = blockColor % 2
				? new StringSpriteComponent{ "XXX" }
				: new StringSpriteComponent{ "###" };
			components.collisionRects[blockID] = new CollisionRectComponent(3, 1);
			components.deleteAfterCollisions[blockID] = new DeleteAfterCollisionComponent{};
			components.scoreWhenHitBlock[blockID] = new ScoreWhenHitBlockComponent{1};

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

		movementSystem(components, secondsSinceLastFrame.count());

		userControlSystem(components, secondsSinceLastFrame.count());

		collisionSystem(components);

		bounceSystem(components);

		scoreBlocksSystem(components, gameScore);
		
		deadBlocksSystem(components);

		renderCharOnConsoleSystem(components, crw);

		renderStringOnConsoleSystem(components, crw);


		//remove temporary components
		for (auto& c : components.moveds) 
			delete c.second;
		components.moveds.clear();
		
		for (auto& c : components.collideds) 
			delete c.second;
		components.collideds.clear();


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