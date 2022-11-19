#include <vector>
#include <chrono>

#include "ConsoleRenderWindow.h"
#include "CollisionRect.h"
#include "Entity.h"

using namespace std::chrono;

bool resolveCollision(CollisionRect& a, CollisionRect& b, Vector& adjustment);



void game(ConsoleRenderWindow& crw) {

	int worldWidth = 30;
	int worldHeight = 60;

	CollisionBoxComponent unitBox;
	std::vector<Entity*> entities;


	Entity ball("ball");
	VelocityComponent vc{ 10,10 };
	ball.addComponent(&vc);

	PositionComponent pc{ 1,20 };
	ball.addComponent(&pc);

	CharSpriteComponent cc{ 'O' };
	ball.addComponent(&cc);

	BounceComponent bc;
	ball.addComponent(&bc);


	ball.addComponent(&unitBox);
	entities.push_back(&ball);

	Entity paddle("paddle");

	PositionComponent ppc{ 10,35 };
	paddle.addComponent(&ppc);

	StringSpriteComponent pcc{ "<====>" };
	paddle.addComponent(&pcc);

	CollisionBoxComponent paddleCollision(6, 1);
	paddle.addComponent(&paddleCollision);


	LeftRightControlComponent plr{ 15 };
	paddle.addComponent(&plr);

	entities.push_back(&paddle);

	CharSpriteComponent hWall('=');
	for (int x = 0; x < worldWidth; x++) {
		Entity* e;
		PositionComponent* pos;
		e = new Entity("wall-top");
		pos = new PositionComponent(x, 0);
		e->addComponent(pos);
		e->addComponent(&hWall);
		e->addComponent(&unitBox);
		entities.push_back(e);

		e = new Entity("wall-bottom");
		pos = new PositionComponent(x, 35);
		e->addComponent(pos);
		e->addComponent(&hWall);
		e->addComponent(&unitBox);
		entities.push_back(e);
	}

	CharSpriteComponent vWall('|');

	for (int y = 1; y < worldHeight - 1; y++) {
		Entity* e;
		PositionComponent* pos;
		e = new Entity("wall-left");
		pos = new PositionComponent(0, y);
		e->addComponent(pos);
		e->addComponent(&vWall);
		e->addComponent(&unitBox);
		entities.push_back(e);

		e = new Entity("wall-right");
		pos = new PositionComponent(worldWidth - 1, y);
		e->addComponent(pos);
		e->addComponent(&vWall);
		e->addComponent(&unitBox);
		entities.push_back(e);

	}

	StringSpriteComponent block1("XXX");
	StringSpriteComponent block2("###");
	CollisionBoxComponent blockCollision(3, 1);
	BlockComponent block;

	int firstBlock = 0;
	for (int x = 3; x < worldWidth - 4; x += 3) {
		int blockColor = firstBlock % 2;
		for (int y = 5; y < 20; y += 1) {
			Entity* e = new Entity("block");
			PositionComponent* pos = new PositionComponent(x, y);
			e->addComponent(pos);
			e->addComponent(blockColor % 2 ? &block1 : &block2);
			e->addComponent(&blockCollision);
			e->addComponent(&block);

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

		MovedComponent* movedComponent = new MovedComponent;
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
			auto lrc = dynamic_cast<LeftRightControlComponent*>(e->getComponent(LeftRightControlComponent::NAME));
			auto pos = dynamic_cast<PositionComponent*>(e->getComponent(PositionComponent::NAME));

			if (lrc && pos) {
				if (bKey[0])
					pos->x += lrc->speed * deltaSeconds;
				if (bKey[1])
					pos->x -= lrc->speed * deltaSeconds;
				e->addComponent(movedComponent);
			}
		}

		CollidedComponent collided;
		for (auto e : entities) {
			auto moved = dynamic_cast<MovedComponent*>(e->getComponent(MovedComponent::NAME));
			auto box = dynamic_cast<CollisionBoxComponent*>(e->getComponent(CollisionBoxComponent::NAME));
			auto pos = dynamic_cast<PositionComponent*>(e->getComponent(PositionComponent::NAME));
			if (moved && box && pos)
				for (auto other : entities) {
					if (other == e) continue;
					auto* other_pos = dynamic_cast<PositionComponent*>(other->getComponent(PositionComponent::NAME));
					auto* other_box = dynamic_cast<CollisionBoxComponent*>(other->getComponent(CollisionBoxComponent::NAME));
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
							CollisionResolvedComponent* crc = new CollisionResolvedComponent(bn.x, bn.y);
							e->addComponent(crc);
							other->addComponent(&collided);
						}
					}
				}
		}

		for (auto e : entities) {
			auto bounce = dynamic_cast<BounceComponent*>(e->getComponent(BounceComponent::NAME));
			auto vel = dynamic_cast<VelocityComponent*>(e->getComponent(VelocityComponent::NAME));
			auto collision = dynamic_cast<CollisionResolvedComponent*>(e->getComponent(CollisionResolvedComponent::NAME));

			if (bounce && vel && collision) {
				if (abs(lround(collision->x)) == 1) vel->x *= -1; //vertical   wall
				if (abs(lround(collision->y)) == 1) vel->y *= -1; //horizontal wall
			}
		}


		for (auto e : entities) {
			auto sprite = dynamic_cast<CharSpriteComponent*>(e->getComponent(CharSpriteComponent::NAME));
			auto pos = dynamic_cast<PositionComponent*>(e->getComponent(PositionComponent::NAME));

			if (sprite && pos) {
				crw.Draw(round(pos->x), round(pos->y), sprite->c);
			}
		}

		for (auto e : entities) {
			auto sprite = dynamic_cast<StringSpriteComponent*>(e->getComponent(StringSpriteComponent::NAME));
			auto pos = dynamic_cast<PositionComponent*>(e->getComponent(PositionComponent::NAME));

			if (sprite && pos) {
				for (int i = 0; sprite->c[i] != '\0'; i++)
					crw.Draw(round(pos->x + i), round(pos->y), sprite->c[i]);
			}
		}

		for (auto e = entities.begin(); e != entities.end();)
		{

			auto collided = dynamic_cast<CollidedComponent*>((*e)->getComponent(CollidedComponent::NAME));
			auto block = dynamic_cast<BlockComponent*>((*e)->getComponent(BlockComponent::NAME));

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