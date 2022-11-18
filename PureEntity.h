#pragma once
#include<string>
#include<map>
#include"component.h"

struct PureEntity {
	std::map<std::string, Component*> components;
public:
	void addComponent(Component* c);
	Component* getComponent(const std::string name);
};