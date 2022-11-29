#include <iostream>
#include <thread>

using namespace std;

#include <stdio.h>
#include <Windows.h>

#include "./ECSGameEngine/ConsoleRenderWindow.h"

void breakoutGame(ConsoleRenderWindow&, int, int);
void marioGame(ConsoleRenderWindow&, int, int);

ConsoleRenderWindow crw;

void playBreaout() {

	int charSizePixels = 10;

	int worldHeight = 40;// Console Screen Size X (columns)
	int worldWidth = 30;// Console Screen Size Y (rows)

	crw.ConstructConsole(worldWidth, worldHeight, charSizePixels, charSizePixels);

	breakoutGame(crw, worldWidth, worldHeight);
}

void playMario() {

	int charSizePixels = 20;

	int worldHeight = 15;// Console Screen Size X (columns)
	int worldWidth = 50;// Console Screen Size Y (rows)

	crw.ConstructConsole(worldWidth, worldHeight, charSizePixels, charSizePixels);

	marioGame(crw, worldWidth, worldHeight);
}

int main() {

	//playBreaout();
	playMario();

	cout << "Game Over!!" << endl;
	system("pause");
	return 0;

}


