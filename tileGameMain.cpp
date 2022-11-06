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


class ConsoleSprite:public Sprite {
	ConsoleRenderWindow& crw;
	char image;
public:
	ConsoleSprite(ConsoleRenderWindow& crw, char image):crw(crw), image(image) {}
	void draw(int x, int y) {
		crw.Draw(x, y, image);
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



int main()
{
	int nScreenWidth = 120;			// Console Screen Size X (columns)
	int nScreenHeight = 30;			// Console Screen Size Y (rows)
	wstring tetromino[7];
	int nFieldWidth = 12;
	int nFieldHeight = 18;
	unsigned char* pField = nullptr;

	ConsoleRenderWindow crw;
	crw.ConstructConsole(50, 50, 10, 10);

	ConsoleSprite es(crw, 'E');
	ConsoleSprite fs(crw, 'F');
	ConsoleSprite ballSprite(crw, '0');

	Entity e(CollisionRect(10,10,1,1),&es);
	e.vel = { 0,0 };

	Ball ball(CollisionRect(10, 18, 1, 1), &ballSprite);
	ball.vel = { 10,-10 };

	ConsoleSpriteFactory csf(crw);

	Level level(
		"===================="
		"=..................="
		"=..................="
		"=.....=.=..........="
		"=.....=.=..........="
		"=.....=.=..........="
		"=======.=====......="
		"=..................="
		"==============.....="
		"=..........=.......="
		"=........===.......="
		"=..................="
		"=........===.......="
		"=..........=.......="
		"=.....======.......="
		"=..................="
		"=..................="
		"=..................="
		"=..................="
		"=..................="
		"=..................="
		"=......=...........="
		"=......=...........="
		"=......=...........="
		"=......=...........="
		"=..................="
		"=..................="
		"=..................."
		"=..................="
		"===================="
		, 20, 30, &csf);

	level.addEntity(&e, "player");
	level.addEntity(&ball, "ball");

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

		crw.DrawString(8, 8, L"Hello");
		//f.draw();
		level.draw();

		crw.Show();
		//crw.Clear();
	}

	cout << "Game Over!! Score:"  << endl;
	system("pause");
	return 0;
}