#include "Level.h"
#include<vector>


Level::Level(const char* map, int width, int height, SpriteFactory& spriteFactory) {
	for (int x = 0; x < width; x++) 
		for (int y = 0; y < height; y++) {
			char tag = map[x + y * width];
			if (tag != '.')
				entities.emplace(string(1, tag), new Entity(CollisionRect(x, y, 1, 1), spriteFactory.create(tag)));
		}
		
}

void Level::addEntity(Entity* entity, string tag) {
	entities.emplace(tag, entity);
}

Entity* Level::getByName(string name)  {
	return entities.find(name)->second;
}

void Level::update(Seconds seconds) {
	for (auto& kv : entities)
		kv.second->update(seconds);

	std::vector<Entity*> checked;
	for (auto& kv : entities) {
		for (auto& e : checked)
			resolveCollision(kv.second->rect, e->rect );
		checked.push_back(kv.second);
	}
	
}

void Level::draw() {
	for (auto& kv : entities)
		kv.second->draw();
}