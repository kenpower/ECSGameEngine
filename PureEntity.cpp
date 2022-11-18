#include "PureEntity.h"

void PureEntity::addComponent(Component* c) {
	components[c->name()] = c;

}

Component* PureEntity::getComponent(const std::string name) {
	auto component = components.find(name);

	return component == components.end() ? NULL : component->second;
}