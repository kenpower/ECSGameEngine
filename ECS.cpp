#define NOMINMAX //https://stackoverflow.com/a/2789510/488802
#include<windows.h>
#include"ECSGameEngine/ECS.h"

EntityID newEntityID() {
	static EntityID id = 0;
	return id++;
}

bool isKeyDown(int keyCode) {
	const short leftMostBit = (short)0x8000; //most significant bit is set if key is down
	return  leftMostBit & GetAsyncKeyState((unsigned char)(keyCode));
}