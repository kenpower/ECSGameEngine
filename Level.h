#pragma once

#include<string>
#include<map>
#include<memory>
#include "Entity.h"

using namespace std;

class SpriteFactory {
public:
	virtual Sprite* create(char) = 0;
};

class Level
{
	std::map<std::string, Entity*> entities;
		
public:
	Level(const char*, int, int, SpriteFactory&);
	Entity* getByName(string name) ;
	void addEntity(Entity*, string);
	void update(chrono::milliseconds);
	void draw();	
};

