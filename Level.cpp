#include "Level.h"

Level::Level(const char* map, int width, int height) {
	for (int x = 0; x < width; x++) 
		for (int y = 0; y < height; y++) {
			char tag = map[x + y * width];
			if (tag != '.')
				entities.emplace(string(1, tag), CollisionRect(x, y, 1, 1));
		}
		
}

const Entity& Level::getByName(string name) const {
	return entities.find(name)->second;
}