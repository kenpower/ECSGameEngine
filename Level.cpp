#include "Level.h"
#include<vector>

Level::Level(const char* map, int width, int height, SpriteFactory* spriteFactory) {
	for (int x = 0; x < width; x++) 
		for (int y = 0; y < height; y++) {
			char tag = map[x + y * width];
			if (tag != '.')
				entities.emplace(string(1, tag)+to_string(x)+string("-")+ to_string(y),
					new Entity(CollisionRect(x, y, 1, 1), spriteFactory->create(tag)));
		}
		
}

void Level::addEntity(Entity* entity, string tag) {
	entities.emplace(tag, entity);
}

Entity* Level::getByName(string name)  {
	return entities.find(name)->second;
}

void Level::update(chrono::milliseconds delta) {
	for (auto& kv : entities)
		kv.second->update(delta);

	std::vector<Entity*> checked;
	for (auto& kv : entities) {
		for (auto& e : checked) {
			bool wasCollision = false;
			Vector adj{ 0,0 };
			if (e->vel >= kv.second->vel) {
				wasCollision = resolveCollision(e->rect, kv.second->rect, adj);
				if(wasCollision) e->collideWith(kv.second, adj);
				
			}
			else {
				wasCollision = resolveCollision(kv.second->rect, e->rect, adj);
				if (wasCollision) kv.second->collideWith(e, adj);
			}
		}
		checked.push_back(kv.second);
	}
	
}

void Level::draw() {
	for (auto& kv : entities)
		kv.second->draw();
}