#include"ECSGameEngine/ECS.h"

EntityID newEntityID() {
	static EntityID id = 0;
	return id++;
}