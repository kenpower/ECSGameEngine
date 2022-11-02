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

	ConsoleSprite s(crw, 'Z');

	Entity e(CollisionRect(10,10,1,1),s);
	e.position = { 10,10 };
	e.vel = { 0.1,0.1 };

	
	// Create Screen Buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	//for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	//HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	//SetConsoleActiveScreenBuffer(hConsole);
	//DWORD dwBytesWritten = 0;

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

		double speed = 0.5;
		x += bKey[0] ? speed : 0;
		x -= bKey[1] ? speed : 0;
		y += bKey[2] ? speed : 0;
		y -= bKey[3] ? speed : 0;

		crw.Draw((int)x, (int)y, 'X');
		crw.DrawString(8, 8, L"Hello");
		e.draw();

		crw.Show();
		crw.Clear();
	}

		cout << "Game Over!! Score:"  << endl;
		system("pause");
		return 0;
}