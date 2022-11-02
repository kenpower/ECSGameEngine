#include <iostream>
#include <thread>
#include <vector>
using namespace std;

#include <stdio.h>
#include <Windows.h>

#include "ConsoleRenderWindow.h"


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
	
	// Create Screen Buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	//for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	//HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	//SetConsoleActiveScreenBuffer(hConsole);
	//DWORD dwBytesWritten = 0;

	bool bGameOver = false;
	bool bKey[4];

	float x = 20, y = 20;
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


		screen[(int)y * nScreenWidth + (int)x] = L'X';
		// Draw Score
		swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", (int)x);

		crw.Draw(5, 5, 'X');
		crw.DrawString(8, 8, L"Hello");
		// Display Frame
		//WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		crw.Show();
	}
		//CloseHandle(hConsole);
		cout << "Game Over!! Score:"  << endl;
		system("pause");
		return 0;
}