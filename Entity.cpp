#include "Entity.h"



void Entity::update(Seconds seconds)
{
	rect.x += vel.x * seconds.value;
	rect.y += vel.y * seconds.value;
}

void Entity::draw()
{
	sprite.draw(position.x, position.y);
}