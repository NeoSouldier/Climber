/*
 *  Platform.h
 *  Climber
 *
 *  Created by Arthur Fox on 16/09/2011.
 *  Copyright 2011 Arthur Fox. All rights reserved.
 *
 */

#ifndef Climber_Platform_h
#define Climber_Platform_h

#include "GameObject.h"
#include "PlatformManager.h"

//Ugly Forward Declaration - Avoidable?
class PlatformManager;

class Platform : public GameObject
{
public:
	//Constructor, allows x to be set originally
	Platform(PlatformManager& rPlatManager, int speed, int initialX = -1, int initialY = -1);
	
	//Moves the platform
	bool virtual move(Uint32 deltaTicks);
	
	//Returns the collision box for the platform
	const virtual CollisionBox collisionBox(Uint32 deltaTicks = 0);
	
	//To change speed of platform
	void setSpeed(int newYVel);
	
	//Checks if platform is visible, if not should be deleted
	bool isVisible();
	
	//Checks if the object in question is on this platform
	bool onPlatform(GameObject* pObject);
	
	//If object went through this platform in last frame, returns y value of platform, -1 otherwise
	float throughPlatform(GameObject* pObject, Uint32 deltaTicks);
};

#endif