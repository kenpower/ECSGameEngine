#include <iostream>
#include <thread>

using namespace std;

#include <stdio.h>
#include <Windows.h>

#include "ConsoleRenderWindow.h"

void game(ConsoleRenderWindow&,int, int);

int main() {
			
	int charSizePixels = 10;

	int worldHeight = 40;// Console Screen Size X (columns)
	int worldWidth = 30;// Console Screen Size Y (rows)

	ConsoleRenderWindow crw;
	crw.ConstructConsole(worldWidth, worldHeight, charSizePixels, charSizePixels);

	game(crw, worldWidth, worldHeight);

	cout << "Game Over!! Score:" << endl;
	system("pause");
	return 0;

}


