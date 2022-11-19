#include <vector>
#include <chrono>
#include <memory>

#include "ConsoleRenderWindow.h"
#include "ECS.h"
#include "BreakoutSystems.h"

using namespace std;
using namespace std::chrono;


enum WallDirection {
	vertical,
	horizontal
};

shared_ptr<Entity> wallEntity(const char* name, double x, double y, WallDirection wd) {
	// TODO use a map
	//auto wall = map<WallDirection, shared_ptr<CharSpriteComponent>>() = {
	//	{WallDirection::horizontal, make_shared<CharSpriteComponent>('=')},
	//	{WallDirection::vertical, make_shared<CharSpriteComponent>('|')},
	//};
	static auto hWall = make_shared<CharSpriteComponent>('=');
	static auto vWall = make_shared<CharSpriteComponent>('|');
	static auto unitBox = make_shared<CollisionBoxComponent>(1, 1);
	
	auto e = make_shared<Entity>(name);
	e->addComponent(make_shared<PositionComponent>(x, y));
	e->addComponent(wd == WallDirection::horizontal ? hWall : vWall);
	e->addComponent(unitBox);
	return e;
}

void game(ConsoleRenderWindow& crw) {

	int worldWidth = 30;
	int worldHeight = 60;
	Entities entities;

	auto unitBox = make_shared<CollisionBoxComponent>(1,1);

	auto ball = make_shared<Entity>("ball");
	ball->addComponent(make_shared<VelocityComponent>(10, 10));
	ball->addComponent(make_shared<PositionComponent>(1, 20));
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


	for (int x = 0; x < worldWidth; x++) {
		entities.push_back(wallEntity("wall-top", x, 0, WallDirection::horizontal));
		entities.push_back(wallEntity("wall-bottom", x, 35, WallDirection::horizontal));
	}

	for (int y = 1; y < worldHeight - 1; y++) {
		entities.push_back(wallEntity("wall-left", 0, y, WallDirection::vertical));
		entities.push_back(wallEntity("wall-right", worldWidth - 1, y, WallDirection::vertical));
	}

	auto block1 = make_shared<StringSpriteComponent>("XXX");
	auto block2 = make_shared<StringSpriteComponent>("###");
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

			entities.push_back(e);
			blockColor++;
		}
		firstBlock++;
	}

	bool bGameOver = false;


	double x = 20, y = 20;
	//float x = 0;
	long count = 0;
	auto begin = std::chrono::high_resolution_clock::now();
	auto start = std::chrono::high_resolution_clock::now();


	while (!bGameOver) { // Main Loop
		auto end = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);


		begin = std::chrono::high_resolution_clock::now();

		chrono::milliseconds frameLength = 10ms;

		// Timing =======================
		//this_thread::sleep_for(frameLength); // Small Step = 1 Game Tick

		// Input ========================


		double speed = 15.0;


		auto movedComponent = make_shared<MovedComponent>();
		double deltaSeconds = frameLength.count() / 1000.0;

		movementSystem(entities, deltaSeconds);

		userControlSystem(entities, deltaSeconds);

		collisionSystem(entities);

		bounceSystem(entities);

		removeDeadBlocks(entities);

		renderCharOnConsoleSystem(entities, crw);

		renderStringOnConsoleSystem(entities, crw);

		

		//remove temporary components
		for (auto e : entities) {
			e->removeComponent(CollisionResolvedComponent::NAME);
			e->removeComponent(MovedComponent::NAME);
			e->removeComponent(CollidedComponent::NAME);

		}

		if (count++ % 1000 == 0) {
			auto now = std::chrono::high_resolution_clock::now();
			auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(now - start);
			wchar_t w_s[100];
			char s[100];
			size_t len = 0;
			int framesPerSec = (float)count / elapsed_time.count();

			_itoa_s(framesPerSec, s, 10);
			mbstowcs_s(&len, w_s, (size_t)30, s, (size_t)30);
			crw.DrawString(1, 40, L"         frames per sec  ");

			crw.DrawString(1, 40, w_s);
		}

		crw.Show();
		crw.Clear();
	}

}