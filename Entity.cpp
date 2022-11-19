#include "Entity.h"

void Entity::addComponent(Component* c) {
	components[c->name()] = c;

}

Component* Entity::getComponent(const std::string name) {
	auto component = components.find(name);

	return component == components.end() ? NULL : component->second;
}

void Entity::removeComponent(const std::string name) {
    for (auto it = components.begin(); it != components.end();)
    {
        if (it->first == name)
            it = components.erase(it);
        else
            ++it;
    }

}

