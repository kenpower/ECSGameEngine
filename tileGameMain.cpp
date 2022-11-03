#include <iostream>
#include <thread>
#include <vector>
using namespace std;

#include <stdio.h>
#include <Windows.h>

#include "ConsoleRenderWindow.h"
#include "Level.h"
#include "Entity.h"


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
	ConsoleSprite gs(crw, 'G');

	Entity e(CollisionRect(10,10,1,1),&es);
	e.vel = { 0,0 };

	Entity f(CollisionRect(11, 11, 1, 1), &fs);
	f.vel = { 0.1,0.1 };

	ConsoleSpriteFactory csf(crw);

	Level level("abcdefghijkl", 3, 4, csf);

	level.addEntity(&e, "player");

	bool bGameOver = false;
	bool bKey[4];

	double x = 20, y = 20;
	//float x = 0;
	while (!bGameOver) // Main Loop
	{
		
		// Timing =======================
		this_thread::sleep_for(50ms); // Small Step = 1 Game Tick

		// Input ========================
		for (int k = 0; k < 4; k++)								// R   L   D    up
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28\x26"[k]))) != 0;

		double speed = 15.0;
		e.vel.x = bKey[0] ? speed : bKey[1] ? -speed : 0;

		e.vel.y = bKey[2] ? speed : bKey[3] ? -speed : 0;


		level.update(Seconds{ 0.05 });

		//crw.Draw((int)x, (int)y, 'X');
		//e.rect = CollisionRect{ x,y,1,1 };
		//f.rect = CollisionRect{ x+1,y+1,1,1 };

		crw.DrawString(8, 8, L"Hello");
		//f.draw();
		level.draw();

		crw.Show();
		crw.Clear();
	}

		cout << "Game Over!! Score:"  << endl;
		system("pause");
		return 0;
}