#include "Entity.h"

Entity::Entity(const CollisionRect rect) : rect(rect)
{
}

void Entity::update(Seconds seconds)
{
	rect.x += vel.x * seconds.value;
	rect.y += vel.y * seconds.value;
}