/*
 *  Platform.cpp
 *  Climber
 *
 *  Created by Arthur Fox on 16/09/2011.
 *  Copyright 2011 Arthur Fox. All rights reserved.
 *
 */

#include "Platform.h"
#include "Global.h"

//Initialise platform variables
Platform::Platform(PlatformManager& rPlatManager, int speed, int initialX, int initialY): GameObject()
{	
	//Gives platform a random picture, sizes 96,128,160 pixels width
	/*imgSize VARIABLE IS USED FOR 2 THINGS: 
	 (1): DECIDING WHICH SIZE THE PLATFORM SHOULD BE
	 (2): ALLOWING THE VALUE OF THE PLATFORM TO BE FURTHER RIGHT IF PLATFORM IS SMALLER*/
    
	//NOTE: UNSURE IF THIS IS THE BEST WAY TO DO IT
    //Loading a new image every time also seems expensive and might be causing the leaks
	int imgSize = rand() % 3;
	switch (imgSize)
	{
		case 2:
			m_pImage = rPlatManager.getPlatformImage(SMALL_PLATFORM);
			break;
		case 1:
			m_pImage = rPlatManager.getPlatformImage(MEDIUM_PLATFORM);
			break;
		case 0:
			m_pImage = rPlatManager.getPlatformImage(LARGE_PLATFORM);
			break;
	}
	
	//Randomly place the platform from x range of 55 - 425+(imgSize*32)
	if(initialX<0)
		m_x = (rand() % (375 + imgSize*32)) +55;
	else 
		m_x = initialX;
	
	if(initialY<0)
		m_y = 0;
	else 
	  m_y = initialY;
	
	//Platforms do not move horizontally so xVel = 0, speed is constant on platforms unlike the player
	m_yVel = speed;
	m_xVel = 0;
}

//Moves the platform vertically 
bool Platform::move(Uint32 deltaTicks)
{
	//Move platform down
	m_y += m_yVel * ( deltaTicks / 1000.f );
	
	//Successfully moved
	return true;
}

//Method which creates a collision box for the platform - for a platform the box is just the image outline
const GameObject::CollisionBox Platform::collisionBox(Uint32 deltaTicks)
{
	//HACK: y+10 is so that the collision box is slightly lower down on the platform, visually looks better
	CollisionBox col = CollisionBox();
	col.x1 = m_x;							// x
	col.x2 = m_x + this->getWidth();		// x + width
	col.y1 = m_y+10;						// y 
	col.y2 = m_y+10 + this->getHeight();  // y + height
	
	return col;
}

//To change speed of platform
void Platform::setSpeed(int newYVel)
{
	m_yVel = newYVel;
}

//Returns true if platform is within screen dimensions
bool Platform::isVisible()
{
	return (m_x >=0 && m_x <=SCREEN_WIDTH && m_y >= 0 && m_y <= SCREEN_HEIGHT);
}

#pragma mark -
#pragma mark CollisionDetection

//Returns true if the object is on this platform - very abstract, as it compares collision boxes which are abstract concepts
bool Platform::onPlatform(GameObject* pObject)
{
	const CollisionBox objCol = pObject->collisionBox();
	const CollisionBox platCol = this->collisionBox();
	
	//Check if object box is inside the platform box
	return (platCol.y1 <= objCol.y1 && objCol.y2 <= platCol.y2 &&
					platCol.x1 <= objCol.x2 && objCol.x1 <= platCol.x2);
}

//Returns the platforms y value if the object has just gone through it, -1 otherwise
float Platform::throughPlatform(GameObject* pObject, Uint32 deltaTicks)
{
	const CollisionBox objCol  = pObject->collisionBox(deltaTicks);
	const CollisionBox platCol = this->collisionBox();
	
	//The platform is considered as a single line, only its top y value is used
	if(objCol.y1 <= platCol.y1 && platCol.y1 <= objCol.y2 &&
		 platCol.x1 <= objCol.x2 && objCol.x1 <= platCol.x2){
		return (m_y+11 - pObject->getHeight());
	}
	
	return CEILING -1;
}