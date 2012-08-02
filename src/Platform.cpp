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

SDL_Surface *Platform::ms_pSmall (0);
SDL_Surface *Platform::ms_pMedium(0);
SDL_Surface *Platform::ms_pLarge (0);

//Initialise platform variables
Platform::Platform(int speed, int initialX, int initialY)
{	
    //The first time a platform is created all the surfaces needed to be correctly initalised
    if (ms_pSmall == 0 || ms_pMedium == 0 || ms_pLarge == 0)
        initPlatformImages();
    
	//Gives platform a random image, sizes 96,128,160 pixels width
	/*imgSize VARIABLE IS USED FOR 2 THINGS: 
	 (1): DECIDING WHICH SIZE THE PLATFORM SHOULD BE
	 (2): ALLOWING THE VALUE OF THE PLATFORM TO BE FURTHER RIGHT IF PLATFORM IS SMALLER*/
	int imgSize = rand() % 3;
	switch (imgSize)
	{
		case 2:
			m_pImage = ms_pSmall;
			break;
		case 1:
			m_pImage = ms_pMedium;
			break;
		case 0:
			m_pImage = ms_pLarge;
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

//Initialise all the static surfaces for the different types of platforms
void Platform::initPlatformImages()
{
    ms_pSmall  = Global::sharedGlobal()->loadImage(RESOURCE_SMALL_PLATFORM) ;
    ms_pMedium = Global::sharedGlobal()->loadImage(RESOURCE_MEDIUM_PLATFORM);
    ms_pLarge  = Global::sharedGlobal()->loadImage(RESOURCE_LARGE_PLATFORM);
    
    if (ms_pSmall == NULL || ms_pMedium == NULL || ms_pLarge == NULL) {
		printf("Could not load platform images correctly: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}
}

//Updates the platforms position
bool Platform::update(Uint32 deltaTicks)
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
bool Platform::onPlatform(GameObject& rObject)
{
	const CollisionBox objCol = rObject.collisionBox();
	const CollisionBox platCol = this->collisionBox();
	
	//Check if object box is inside the platform box
	return (platCol.y1 <= objCol.y1 && objCol.y2 <= platCol.y2 &&
					platCol.x1 <= objCol.x2 && objCol.x1 <= platCol.x2);
}

//Returns the platforms y value if the object has just gone through it, -1 otherwise
float Platform::throughPlatform(GameObject& rObject, Uint32 deltaTicks)
{
	const CollisionBox objCol  = rObject.collisionBox(deltaTicks);
	const CollisionBox platCol = this->collisionBox();
	
	//The platform is considered as a single line, only its top y value is used
	if(objCol.y1 <= platCol.y1 && platCol.y1 <= objCol.y2 &&
		 platCol.x1 <= objCol.x2 && objCol.x1 <= platCol.x2){
		return (m_y+11 - rObject.getHeight());
	}
	
	return CEILING -1;
}