#pragma once
#include<map>

#include"ECSComponents.h"
#include"../BreakoutComponents.h"
#include"../MarioComponents.h"


using namespace std;

typedef int EntityID;

EntityID newEntityID();

bool isKeyDown(int keyCode);

struct Components {
	map<EntityID, PositionComponent*> positions;
	map<EntityID, VelocityComponent*> velocitys;
	map<EntityID, CollisionRectComponent*> collisionRects;
	map<EntityID, CharSpriteComponent*> charSprites;
	map<EntityID, StringSpriteComponent*> stringSprites;
	map<EntityID, HasCollidedComponent*> collideds;
	map<EntityID, HasMovedComponent*> moveds;
	
	//Breakout
	map<EntityID, DeleteAfterCollisionComponent*> deleteAfterCollisions;
	map<EntityID, LeftRightControlComponent*> leftRightControls;
	map<EntityID, BounceComponent*> bounces;
	map<EntityID, ScoreWhenHitBlockComponent*> scoreWhenHitBlock;
	map<EntityID, PaddleBounceComponent*> paddleBounces;

	//Mario
	map<EntityID, GravityComponent*> gravities;
	map<EntityID, JumpComponent*> jumps;
	//map<EntityID, OnFloorComponent*> onFloors;




	void deleteEntity(EntityID id) {
		positions.erase(id);
		velocitys.erase(id);
		collisionRects.erase(id);
		collideds.erase(id);
		charSprites.erase(id);
		stringSprites.erase(id);
		moveds.erase(id);

		//Breakout
		bounces.erase(id);
		leftRightControls.erase(id);
		scoreWhenHitBlock.erase(id);
		deleteAfterCollisions.erase(id);
		paddleBounces.erase(id);
		
		//Mario
		gravities.erase(id);
		jumps.erase(id);
		//onFloors.erase(id);

	};
};