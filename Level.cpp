#include "Level.h"

Level::Level(const char* map, int width, int height) {
	for (int x = 0; x < width; x++) 
		for (int y = 0; y < height; y++) {
			char tag = map[x + y * width];
			if (tag != '.')
				rects.emplace(string(1, tag), CollisionRect(x, y, 1, 1));
		}
		
}

const CollisionRect& Level::getByName(string name) const {
	return rects.find(name)->second;
}