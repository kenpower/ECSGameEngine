#pragma once
#include<string>
#include<map>
#include"component.h"

struct Entity {
	std::map<std::string, Component*> components;
	const std::string name;
public:
	Entity(const std::string name) :name(name) {}
	void addComponent(Component* c);
	void removeComponent(const std::string name);

	Component* getComponent(const std::string name);
	
	/* Uses advanced c++ feature(generics), gets the comonent based on type and automatically downcasts it*/
	/* Note templated function need tobe defined in class */
	template<class DerivedComponent>
	DerivedComponent* getComponent() {
		Component* c = getComponent(DerivedComponent::NAME);
		return dynamic_cast<DerivedComponent*>(c);
	}
};