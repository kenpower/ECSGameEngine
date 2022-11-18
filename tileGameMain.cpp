#include <iostream>
#include <thread>
#include <vector>
using namespace std;

#include <stdio.h>
#include <Windows.h>

#include "ConsoleRenderWindow.h"
#include "Level.h"
#include "Entity.h"
#include "Ball.h"
#include "PureEntity.h"

bool resolveCollision(CollisionRect& a, CollisionRect& b, Vector& adjustment);

class ConsoleSprite:public Sprite {
	ConsoleRenderWindow& crw;
	const char* image;
	int width;
	int height;
public:
	ConsoleSprite(ConsoleRenderWindow& crw, char image) :crw(crw), image{ new char[1]{image} }, width{ 1 }, height{ 1 } {}
	ConsoleSprite(ConsoleRenderWindow& crw, const char* image, int width, int height) :crw(crw), image(image), width{ width }, height{ height } {}
	void draw(int x, int y) {
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
				crw.Draw(x + i, y + j, image[i + j * width]);
	}
};

class ConsoleSpriteFactory : public SpriteFactory {
	ConsoleRenderWindow& crw;
public:
	ConsoleSpriteFactory(ConsoleRenderWindow& crw) :crw(crw) {}
	Sprite* create(char tag) {
		return new ConsoleSprite(crw, tag);
	}
};

class Block :public Entity {

public:
	using Entity::Entity; //use parent constructor

	void collideWith(Entity* other, Vector adjustment) {
		active = false;
	}


};

void newGame(ConsoleRenderWindow& crw);

int main() {
	int nScreenWidth = 120;			// Console Screen Size X (columns)
	int nScreenHeight = 30;			// Console Screen Size Y (rows)
	wstring tetromino[7];
	int nFieldWidth = 12;
	int nFieldHeight = 18;
	unsigned char* pField = nullptr;

	int worldHeight = 60;
	int worldWidth = 30;

	ConsoleRenderWindow crw;
	crw.ConstructConsole(worldWidth, worldHeight, 10, 10);

	//oldGame(crw);
	newGame(crw);

	cout << "Game Over!! Score:" << endl;
	system("pause");
	return 0;

}

void newGame(ConsoleRenderWindow& crw) {

	int worldWidth = 30;
	int worldHeight = 60;

	CollisionBoxComponent unitBox;
	std::vector<PureEntity*> entities;

	
		PureEntity ball("ball");
		VelocityComponent vc{ 10,10 };
		ball.addComponent(&vc);

		PositionComponent pc{ 10,10 };
		ball.addComponent(&pc);

		CharSpriteComponent cc{ 'O' };
		ball.addComponent(&cc);

		BounceComponent bc;
		ball.addComponent(&bc);


		ball.addComponent(&unitBox);
		entities.push_back(&ball);
	
		PureEntity paddle("paddle");

		PositionComponent ppc{ 10,25 };
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
		PureEntity* e;
		PositionComponent* pos;
		e = new PureEntity("wall-top");
		pos = new PositionComponent(x, 0);
		e->addComponent(pos);
		e->addComponent(&hWall);
		e->addComponent(&unitBox);
		entities.push_back(e);

		e = new PureEntity("wall-bottom");
		pos = new PositionComponent(x, 35);
		e->addComponent(pos);
		e->addComponent(&hWall);
		e->addComponent(&unitBox);
		entities.push_back(e);
	}
	
	CharSpriteComponent vWall('|');

	for (int y = 1; y < worldHeight - 1; y++) {
		PureEntity* e;
		PositionComponent* pos;
		e = new PureEntity("wall-left");
		pos = new PositionComponent(0, y);
		e->addComponent(pos);
		e->addComponent(&vWall);
		e->addComponent(&unitBox);
		entities.push_back(e);

		e = new PureEntity("wall-right");
		pos = new PositionComponent(worldWidth - 1, y);
		e->addComponent(pos);
		e->addComponent(&vWall);
		e->addComponent(&unitBox);
		entities.push_back(e);

	}

	bool bGameOver = false;
	bool bKey[4];

	double x = 20, y = 20;
	//float x = 0;
	long count = 0;
	auto begin = std::chrono::high_resolution_clock::now();
	auto start = std::chrono::high_resolution_clock::now();


	while (!bGameOver){ // Main Loop
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
		for (auto e : entities){
			auto* vel = dynamic_cast<VelocityComponent*>(e->getComponent(VelocityComponent::NAME));
			auto* pos = dynamic_cast<PositionComponent*>(e->getComponent(PositionComponent::NAME));

			if (vel && pos) {
				pos->x += vel->x * deltaSeconds;
				pos->y += vel->y * deltaSeconds;
				e->addComponent(movedComponent);
			}
		}

		for (auto e : entities) {
			auto* lrc = dynamic_cast<LeftRightControlComponent*>(e->getComponent(LeftRightControlComponent::NAME));
			auto* pos = dynamic_cast<PositionComponent*>(e->getComponent(PositionComponent::NAME));

			if (lrc && pos) {
				if(bKey[0])
					pos->x += lrc->speed * deltaSeconds;
				if (bKey[1])
					pos->x -= lrc->speed * deltaSeconds;
				e->addComponent(movedComponent);
			}
		}

		for (auto e : entities) {
			auto* moved = dynamic_cast<MovedComponent*>(e->getComponent(MovedComponent::NAME));
			auto* box = dynamic_cast<CollisionBoxComponent*>(e->getComponent(CollisionBoxComponent::NAME));
			auto* pos = dynamic_cast<PositionComponent*>(e->getComponent(PositionComponent::NAME));
			if(moved && box && pos)
				for (auto  other: entities) {
					if (other == e) continue;
					auto* other_pos = dynamic_cast<PositionComponent*>(other->getComponent(PositionComponent::NAME));
					auto* other_box = dynamic_cast<CollisionBoxComponent*>(other->getComponent(CollisionBoxComponent::NAME));
					if (other_pos && other_box) {
						CollisionRect moving{ pos->x, pos->y, box->w, box->h };
						CollisionRect other_r{ other_pos->x, other_pos->y, other_box->w, other_box->h };
						Vector adj{0,0};
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
						}
					}
				}
		}

		for (auto e : entities) {
			auto* bounce = dynamic_cast<BounceComponent*>(e->getComponent(BounceComponent::NAME));
			auto* vel = dynamic_cast<VelocityComponent*>(e->getComponent(VelocityComponent::NAME));
			auto* collision = dynamic_cast<CollisionResolvedComponent*>(e->getComponent(CollisionResolvedComponent::NAME));

			if (bounce && vel && collision) {
				if (abs(lround(collision->x)) == 1) vel->x *= -1; //vertical   wall
				if (abs(lround(collision->y)) == 1) vel->y *= -1; //horizontal wall
			}
		}


		for (auto e : entities) {
			auto* sprite = dynamic_cast<CharSpriteComponent*>(e->getComponent(CharSpriteComponent::NAME));
			auto* pos = dynamic_cast<PositionComponent*>(e->getComponent(PositionComponent::NAME));

			if (sprite && pos) {
				crw.Draw(round(pos->x), round(pos->y), sprite->c);
			}
		}

		for (auto e : entities) {
			auto* sprite = dynamic_cast<StringSpriteComponent*>(e->getComponent(StringSpriteComponent::NAME));
			auto* pos = dynamic_cast<PositionComponent*>(e->getComponent(PositionComponent::NAME));

			if (sprite && pos) {
				for (int i = 0; sprite->c[i] != '\0';i++)
					crw.Draw(round(pos->x+i), round(pos->y), sprite->c[i]);
			}
		}

		//remove temp components
		for (auto e : entities) {
			e->removeComponent(CollisionResolvedComponent::NAME);
			e->removeComponent(MovedComponent::NAME);
		}

		if (count++ % 1000 == 0) {
			auto now = std::chrono::high_resolution_clock::now();
			auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(now - start);
			wchar_t w_s[100];
			char s[100];
			size_t len=0;
			int framesPerSec = (float)count / elapsed_time.count();
			
			_itoa_s(framesPerSec,s, 10);
			mbstowcs_s(&len, w_s, (size_t)30, s, (size_t)30);
			crw.DrawString(1, 40, L"         frames per sec  ");

			crw.DrawString(1, 40, w_s);
		}

		crw.Show();
		//crw.Clear();
	}

}

void oldGame(ConsoleRenderWindow& crw)
{


	ConsoleSprite es(crw, "<===>", 5, 1);
	ConsoleSprite fs(crw, 'F');
	ConsoleSprite ballSprite(crw, "O", 1, 1);

	int worldWidth = 30;
	int worldHeight = 60;
	Entity e(CollisionRect((worldWidth - 5) / 2, worldHeight * 0.9, 5, 1), &es);
	e.vel = { 0,0 };

	Ball ball(CollisionRect(10, 22, 1, 1), &ballSprite);
	ball.vel = { 75,-75 };

	ConsoleSpriteFactory csf(crw);

	//Level level(
	//	"===================="
	//	"=..................="
	//	"=..................="
	//	"=.....=.=..........="
	//	"=.....=.=..........="
	//	"=.....=.=..........="
	//	"=======.=====......="
	//	"=..................="
	//	"==============.....="
	//	"=..........=.......="
	//	"=........===.......="
	//	"=..................="
	//	"=........===.......="
	//	"=..........=.......="
	//	"=.....======.......="
	//	"=..................="
	//	"=..................="
	//	"=..................="
	//	"=..................="
	//	"=..................="
	//	"=..................="
	//	"=......=...........="
	//	"=......=...........="
	//	"=......=...........="
	//	"=......=...........="
	//	"=..................="
	//	"=..................="
	//	"=..................."
	//	"=..................="
	//	"===================="
	//	, worldWidth, worldHeight, &csf);

	Level level;
	level.addEntity(&e, "player");
	level.addEntity(&ball, "ball");

	Entity* wall;
	ConsoleSprite hWalls(crw, '=');
	int wallCount = 0;
	for (int x = 0; x < worldWidth; x++) {
		wall = new Entity(CollisionRect(x, 0, 1, 1), &hWalls);
		level.addEntity(wall, "wall" + to_string(wallCount++));
		wall = new Entity(CollisionRect(x, 25 /*worldHeight - 1*/, 1, 1), &hWalls);
		level.addEntity(wall, "wall" + to_string(wallCount++));

	}
	ConsoleSprite vWalls(crw, '|');
	for (int y = 1; y <  worldHeight - 1; y++) {
		wall = new Entity(CollisionRect(0, y, 1, 1), &vWalls);
		level.addEntity(wall, "wall" + to_string(wallCount++));
		wall = new Entity(CollisionRect(worldWidth - 1, y, 1, 1), &vWalls);
		level.addEntity(wall, "wall" + to_string(wallCount++));

	}

	ConsoleSprite block1(crw, "XXX", 3, 1);
	ConsoleSprite block2(crw, "###", 3, 1);
	Block* block;
	int firstBlock = 0;
	for (int x = 3; x < worldWidth - 4; x += 3) {
		int blockColor = firstBlock % 2;
		for (int y = 5; y < 20; y += 1) {
			block = blockColor % 2
				? new Block(CollisionRect(x, y, 3, 1), &block1)
				: new Block(CollisionRect(x, y, 3, 1), &block2);
			level.addEntity(block, "block" + to_string(x)+"-"+to_string(y));
			blockColor++;
		}
		firstBlock++;
	}


	bool bGameOver = false;
	bool bKey[4];

	double x = 20, y = 20;
	//float x = 0;
	while (!bGameOver) // Main Loop
	{
		chrono::milliseconds frameLength = 10ms;
		
		// Timing =======================
		this_thread::sleep_for(frameLength); // Small Step = 1 Game Tick

		// Input ========================
		for (int k = 0; k < 4; k++)								// R   L   D    up
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28\x26"[k]))) != 0;

		double speed = 15.0;
		e.vel.x = bKey[0] ? speed : bKey[1] ? -speed : 0;

		e.vel.y = bKey[2] ? speed : bKey[3] ? -speed : 0;


		level.update(frameLength);

		//crw.Draw((int)x, (int)y, 'X');
		//e.rect = CollisionRect{ x,y,1,1 };
		//f.rect = CollisionRect{ x+1,y+1,1,1 };

		//crw.DrawString(8, 8, L"Hello");
		//f.draw();
		level.draw();

		crw.Show();
		crw.Clear();
	}


}