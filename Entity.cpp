#include "Entity.h"



void Entity::update(std::chrono::milliseconds ms)
{
	rect.x += vel.x * ms.count()/1000.0;
	rect.y += vel.y * ms.count()/1000.0;
}

void Entity::draw()
{
	sprite->draw(round(rect.x), round(rect.y));
}