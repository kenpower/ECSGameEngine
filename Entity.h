#pragma once
#include<string>
#include<map>
#include<memory>
#include"component.h"

using namespace std;

//struct Entity {
//
//	map<string, shared_ptr<Component>> components;
//	const string name;
//public:
//	Entity(const string name) :name(name) {}
//	void removeComponent(const string name);
//
//	void addComponent(shared_ptr<Component> c) {
//		components[c->name()] = c;
//	}
//
//	shared_ptr<Component> getComponent(const string name);
//
//	/* Uses advanced c++ feature(generics), gets the comonent based on type and automatically downcasts it*/
//	/* Note templated function need tobe defined in class */
//	template<class DerivedComponent>
//	shared_ptr<DerivedComponent> getComponent() {
//		shared_ptr<Component> c = getComponent(DerivedComponent::NAME);
//		return dynamic_pointer_cast<DerivedComponent>(c);
//	}
//};
