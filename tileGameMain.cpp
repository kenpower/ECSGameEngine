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

	std::vector<PureEntity*> entities;

	PureEntity ball;
	VelocityComponent vc{ 10,10 };
	ball.addComponent(&vc);

	PositionComponent pc{ 10,10 };
	ball.addComponent(&pc);

	CharSpriteComponent cc{ 'O' };
	ball.addComponent(&cc);
	entities.push_back(&ball);

	PureEntity paddle;

	PositionComponent ppc{ 10,25 };
	paddle.addComponent(&ppc);

	CharSpriteComponent pcc{ '=' };
	paddle.addComponent(&cc);

	LeftRightControlComponent plr{15 };
	paddle.addComponent(&plr);

	entities.push_back(&paddle); 
	
	CharSpriteComponent hWall('=');
	for (int x = 0; x < worldWidth; x++) {
		PureEntity* e;
		PositionComponent* pos;
		e = new PureEntity;
		pos = new PositionComponent(x, 0);
		e->addComponent(pos);
		e->addComponent(&hWall);
		entities.push_back(e);

		e = new PureEntity;
		pos = new PositionComponent(x, 35);
		e->addComponent(pos);
		e->addComponent(&hWall);
		entities.push_back(e);

		//wall = new Entity(CollisionRect(x, 0, 1, 1), &hWalls);
		//level.addEntity(wall, "wall" + to_string(wallCount++));
		//wall = new Entity(CollisionRect(x, 25 /*worldHeight - 1*/, 1, 1), &hWalls);

	}
	
	CharSpriteComponent vWall('|');

	for (int y = 1; y < worldHeight - 1; y++) {
		PureEntity* e;
		PositionComponent* pos;
		e = new PureEntity;
		pos = new PositionComponent(0, y);
		e->addComponent(pos);
		e->addComponent(&vWall);
		entities.push_back(e);

		e = new PureEntity;
		pos = new PositionComponent(worldWidth - 1, y);
		e->addComponent(pos);
		e->addComponent(&vWall);
		entities.push_back(e);
		//wall = new Entity(CollisionRect(0, y, 1, 1), &vWalls);
		//level.addEntity(wall, "wall" + to_string(wallCount++));
		//wall = new Entity(CollisionRect(worldWidth - 1, y, 1, 1), &vWalls);
		//level.addEntity(wall, "wall" + to_string(wallCount++));

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
		//e.vel.x = bKey[0] ? speed : bKey[1] ? -speed : 0;

		//e.vel.y = bKey[2] ? speed : bKey[3] ? -speed : 0;

		double deltaSeconds = frameLength.count() / 1000.0;
		for (auto e : entities){
			auto* vel = dynamic_cast<VelocityComponent*>(e->getComponent(VelocityComponent::NAME));
			auto* pos = dynamic_cast<PositionComponent*>(e->getComponent(PositionComponent::NAME));

			if (vel && pos) {
				pos->x += vel->x * deltaSeconds;
				pos->y += vel->y * deltaSeconds;
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
			}
		}

		//level.update(frameLength);

		for (auto e : entities) {
			auto* sprite = dynamic_cast<CharSpriteComponent*>(e->getComponent(CharSpriteComponent::NAME));
			auto* pos = dynamic_cast<PositionComponent*>(e->getComponent(PositionComponent::NAME));

			if (sprite && pos) {
				crw.Draw(round(pos->x), round(pos->y), sprite->c);
			}
		}

		crw.Show();
		crw.Clear();
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