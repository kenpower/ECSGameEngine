#include <iostream>
#include <thread>

using namespace std;

#include <stdio.h>
#include <Windows.h>

#include "./ECSGameEngine/ConsoleRenderWindow.h"

void game(ConsoleRenderWindow&,int, int);
#include <map>

int main() {
		
	//map<int, int> m;
	//m[5] = 42;
	//for (auto i : m)
	//	std::cout << i.second << '/n';

	//system("pause");

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


