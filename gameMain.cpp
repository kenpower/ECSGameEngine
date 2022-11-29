#include <iostream>
#include <thread>

using namespace std;

#include <stdio.h>
#include <Windows.h>

#include "./ECSGameEngine/ConsoleRenderWindow.h"

void breakoutGame(ConsoleRenderWindow&,int, int);
#include <map>

ConsoleRenderWindow crw;

void playBreaout() {

	int charSizePixels = 10;

	int worldHeight = 40;// Console Screen Size X (columns)
	int worldWidth = 30;// Console Screen Size Y (rows)

	crw.ConstructConsole(worldWidth, worldHeight, charSizePixels, charSizePixels);

	breakoutGame(crw, worldWidth, worldHeight);
}

int main() {

	playBreaout();

	cout << "Game Over!!" << endl;
	system("pause");
	return 0;

}


