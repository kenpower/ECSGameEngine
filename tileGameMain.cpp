#include <iostream>
#include <thread>

using namespace std;

#include <stdio.h>
#include <Windows.h>

#include "ConsoleRenderWindow.h"

void game(ConsoleRenderWindow& crw);

int main() {
			
	int charSizePixels = 10;

	int worldHeight = 60;// Console Screen Size X (columns)
	int worldWidth = 30;// Console Screen Size Y (rows)

	ConsoleRenderWindow crw;
	crw.ConstructConsole(worldWidth, worldHeight, charSizePixels, charSizePixels);

	game(crw);

	cout << "Game Over!! Score:" << endl;
	system("pause");
	return 0;

}


