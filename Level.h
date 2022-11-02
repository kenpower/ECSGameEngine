#pragma once

#include<string>
#include<map>
#include<memory>
#include "Entity.h"

using namespace std;

class Level
{
	std::map<std::string, Entity> entities;
		
public:
	Level(const char*, int, int);
	Entity& getByName(string name) ;
	void update(Seconds);
	void draw();
};

