#include <vector>
#include <chrono>
#include <memory>

using namespace std;

#include "ConsoleRenderWindow.h"
#include "CollisionRect.h"
#include "Entity.h"

using namespace std::chrono;

bool resolveCollision(CollisionRect& a, CollisionRect& b, Vector& adjustment);

//unique_ptr<Entity> createBall() {
//	unique_ptr<Entity> e(new Entity("ball"));
//}

void game(ConsoleRenderWindow& crw) {

	int worldWidth = 30;
	int worldHeight = 60;
	std::vector<Entity*> entities;

	auto unitBox = make_shared<CollisionBoxComponent>(1,1);

	Entity ball("ball");
	ball.addComponent(make_shared<VelocityComponent>(10, 10));
	ball.addComponent(make_shared<PositionComponent>(1, 20));
	ball.addComponent(make_shared<CharSpriteComponent>('O'));
	ball.addComponent(make_shared<BounceComponent>());
	ball.addComponent(unitBox);
	entities.push_back(&ball);

	Entity paddle("paddle");
	paddle.addComponent(make_shared<PositionComponent>(10, 35));
	paddle.addComponent(make_shared<StringSpriteComponent>("<====>"));
	paddle.addComponent(make_shared<CollisionBoxComponent>(6,1));
	paddle.addComponent(make_shared<LeftRightControlComponent>(15));
	entities.push_back(&paddle);

	auto hWall = make_shared<CharSpriteComponent>('=');
	auto vWall = make_shared<CharSpriteComponent>('|');
	for (int x = 0; x < worldWidth; x++) {
		Entity* e;
		e = new Entity("wall-top");
		e->addComponent(make_shared<PositionComponent>(x, 0));
		e->addComponent(hWall);
		e->addComponent(unitBox);
		entities.push_back(e);

		e = new Entity("wall-bottom");
		e->addComponent(make_shared<PositionComponent>(x, 35));
		e->addComponent(hWall);
		e->addComponent(unitBox);
		entities.push_back(e);
	}

	for (int y = 1; y < worldHeight - 1; y++) {
		Entity* e;
		e = new Entity("wall-left");
		e->addComponent(make_shared<PositionComponent>(0, y));
		e->addComponent(vWall);
		e->addComponent(unitBox);
		entities.push_back(e);

		e = new Entity("wall-right");
		e->addComponent(make_shared<PositionComponent>(worldWidth - 1, y));
		e->addComponent(vWall);
		e->addComponent(unitBox);
		entities.push_back(e);

	}

	auto block1 = make_shared<StringSpriteComponent>("XXX");
	auto block2 = make_shared<StringSpriteComponent>("###");
	auto blockCollision = make_shared<CollisionBoxComponent>(3, 1);
	auto block = make_shared<BlockComponent>();

	int firstBlock = 0;
	for (int x = 3; x < worldWidth - 4; x += 3) {
		int blockColor = firstBlock % 2;
		for (int y = 5; y < 20; y += 1) {
			Entity* e = new Entity("block");
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
	bool bKey[4];

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
		for (int k = 0; k < 4; k++)								// R   L   D    up
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28\x26"[k]))) != 0;

		double speed = 15.0;
		//e.vel.x = bKey[0] ? speed : bKey[1] ? -speed : 0;

		//e.vel.y = bKey[2] ? speed : bKey[3] ? -speed : 0;

		auto movedComponent = make_shared<MovedComponent>();
		double deltaSeconds = frameLength.count() / 1000.0;

		for (auto e : entities) {
			//auto* vel = dynamic_cast<VelocityComponent*>(e->getComponent(VelocityComponent::NAME));
			//auto* pos = dynamic_cast<PositionComponent*>(e->getComponent(PositionComponent::NAME));
			auto vel = e->getComponent<VelocityComponent>();
			auto pos = e->getComponent<PositionComponent>();

			if (vel && pos) {
				pos->x += vel->x * deltaSeconds;
				pos->y += vel->y * deltaSeconds;
				e->addComponent(movedComponent);
			}
		}

		for (auto e : entities) {
			auto lrc = dynamic_pointer_cast<LeftRightControlComponent>(e->getComponent(LeftRightControlComponent::NAME));
			auto pos = dynamic_pointer_cast<PositionComponent>(e->getComponent(PositionComponent::NAME));

			if (lrc && pos) {
				if (bKey[0])
					pos->x += lrc->speed * deltaSeconds;
				if (bKey[1])
					pos->x -= lrc->speed * deltaSeconds;
				e->addComponent(movedComponent);
			}
		}

		auto collided = make_shared<CollidedComponent>();
		for (auto e : entities) {
			auto moved = dynamic_pointer_cast<MovedComponent>(e->getComponent(MovedComponent::NAME));
			auto box = dynamic_pointer_cast<CollisionBoxComponent>(e->getComponent(CollisionBoxComponent::NAME));
			auto pos = dynamic_pointer_cast<PositionComponent>(e->getComponent(PositionComponent::NAME));
			if (moved && box && pos)
				for (auto other : entities) {
					if (other == e) continue;
					auto other_pos = dynamic_pointer_cast<PositionComponent>(other->getComponent(PositionComponent::NAME));
					auto other_box = dynamic_pointer_cast<CollisionBoxComponent>(other->getComponent(CollisionBoxComponent::NAME));
					if (other_pos && other_box) {
						CollisionRect moving{ pos->x, pos->y, box->w, box->h };
						CollisionRect other_r{ other_pos->x, other_pos->y, other_box->w, other_box->h };
						Vector adj{ 0,0 };
						bool collision = resolveCollision(moving, other_r, adj);
						if (collision) {
							pos->x += adj.x;
							pos->y += adj.y;
							Vector bn;
							if (fabs(adj.x) > fabs(adj.y))
								bn = (adj.x > 0) ? Vector{ 1, 0 } : Vector{ -1, 0 };
							else
								bn = (adj.y > 0) ? Vector{ 0,1 } : Vector{ 0,-1 };
							auto crc = make_shared<CollisionResolvedComponent>(bn.x, bn.y);
							e->addComponent(crc);
							other->addComponent(collided);
						}
					}
				}
		}

		for (auto e : entities) {
			auto bounce = dynamic_pointer_cast<BounceComponent>(e->getComponent(BounceComponent::NAME));
			auto vel = dynamic_pointer_cast<VelocityComponent>(e->getComponent(VelocityComponent::NAME));
			auto collision = dynamic_pointer_cast<CollisionResolvedComponent>(e->getComponent(CollisionResolvedComponent::NAME));

			if (bounce && vel && collision) {
				if (abs(lround(collision->x)) == 1) vel->x *= -1; //vertical   wall
				if (abs(lround(collision->y)) == 1) vel->y *= -1; //horizontal wall
			}
		}


		for (auto e : entities) {
			auto sprite = dynamic_pointer_cast<CharSpriteComponent>(e->getComponent(CharSpriteComponent::NAME));
			auto pos = dynamic_pointer_cast<PositionComponent>(e->getComponent(PositionComponent::NAME));

			if (sprite && pos) {
				crw.Draw(round(pos->x), round(pos->y), sprite->c);
			}
		}

		for (auto e : entities) {
			auto sprite = dynamic_pointer_cast<StringSpriteComponent>(e->getComponent(StringSpriteComponent::NAME));
			auto pos = dynamic_pointer_cast<PositionComponent>(e->getComponent(PositionComponent::NAME));

			if (sprite && pos) {
				for (int i = 0; sprite->c[i] != '\0'; i++)
					crw.Draw(round(pos->x + i), round(pos->y), sprite->c[i]);
			}
		}

		for (auto e = entities.begin(); e != entities.end();)
		{

			auto collided = dynamic_pointer_cast<CollidedComponent>((*e)->getComponent(CollidedComponent::NAME));
			auto block = dynamic_pointer_cast<BlockComponent>((*e)->getComponent(BlockComponent::NAME));

			if (collided && block)
				e = entities.erase(e);
			else
				++e;
		}

		//remove temp components
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