#include "Level.h"

Level::Level(const char* map, int width, int height) {
	for (int x = 0; x < width; x++) 
		for (int y = 0; y < height; y++) {
			char tag = map[x + y * width];
			if (tag != '.')
				entities.emplace(string(1, tag), CollisionRect(x, y, 1, 1));
		}
		
}

Entity& Level::getByName(string name)  {
	return entities.find(name)->second;
}

void Level::update(Seconds seconds) {
	for (auto kv : entities) 
		kv.second.update(seconds);

	
}