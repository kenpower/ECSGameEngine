#include "PureEntity.h"

void PureEntity::addComponent(Component* c) {
	components[c->name()] = c;

}

Component* PureEntity::getComponent(const std::string name) {
	auto component = components.find(name);

	return component == components.end() ? NULL : component->second;
}

void PureEntity::removeComponent(const std::string name) {
    for (auto it = components.begin(); it != components.end();)
    {
        if (it->first == name)
            it = components.erase(it);
        else
            ++it;
    }

}

