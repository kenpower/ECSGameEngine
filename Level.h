#pragma once

#include<string>
#include<map>
#include<memory>
#include "CollisionRect.h"

using namespace std;

class Level
{
	std::map<std::string, CollisionRect> rects;
		
public:
	Level(const char*, int, int);
	const CollisionRect& getByName(string name) const;
};

