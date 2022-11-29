#pragma once
#include<map>
#include"ECSComponents.h"
#include"../BreakoutComponents.h"
#include"../MarioComponents.h"


using namespace std;

typedef int EntityID;

EntityID newEntityID();

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


	void deleteEntity(EntityID id) {
		positions.erase(id);
		velocitys.erase(id);
		collisionRects.erase(id);
		charSprites.erase(id);
		stringSprites.erase(id);
		leftRightControls.erase(id);
		bounces.erase(id);
		scoreWhenHitBlock.erase(id);
		deleteAfterCollisions.erase(id);
		collideds.erase(id);
		moveds.erase(id);
		paddleBounces.erase(id);
		gravities.erase(id);

	};
};