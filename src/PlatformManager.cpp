/*
 *  PlatformManager.cpp
 *  Climber
 *
 *  Created by Arthur Fox on 17/09/2011.
 *  Copyright 2011 Arthur Fox. All rights reserved.
 *
 */

#include "PlatformManager.h"
#include "Global.h"
#include <iostream>

const int INIT_SPEED = 124; // Initial platform speed

//initialise platform array and speed
PlatformManager::PlatformManager()
{
	speed = INIT_SPEED;
    
    m_pSmall  = Global::sharedGlobal()->loadImage(RESOURCE_SMALL_PLATFORM);
    m_pMedium = Global::sharedGlobal()->loadImage(RESOURCE_MEDIUM_PLATFORM);
    m_pLarge  = Global::sharedGlobal()->loadImage(RESOURCE_LARGE_PLATFORM);
    
	newPlatform(SCREEN_WIDTH/2 - 60 , PLAYER_HEIGHT);
}

//Returns the 
SDL_Surface* PlatformManager::getPlatformImage(int platform)
{
    switch (platform) 
    {
        case SMALL_PLATFORM:
            return m_pSmall;
            break;
            
        case MEDIUM_PLATFORM:
            return m_pMedium;
            break;
            
        case LARGE_PLATFORM:
            return m_pLarge;
            break;
            
        default:
            return m_pSmall;
            break;
    }
}

//Only public access method which adds a new platform to the platforms vector 
//Ensures restriction that platforms are at least 100 pixels apart
bool PlatformManager::addPlatform(int initialX, int initialY)
{
	if (platforms.empty() || (platforms.back().getY()>= DISTANCE_BETWEEN_PLATFORMS) )
    {
		newPlatform(initialX, initialY);
		return true;
	}
	return false;
}

//Private function for adding a new platform to the platforms vector 
void PlatformManager::newPlatform(int initialX, int initialY)
{
	Platform plat(*this, speed, initialX, initialY);
	platforms.push_back(plat);
}

//Returns speed of platforms
int PlatformManager::getSpeed()
{
	return speed;
}

//Sets speed of all platforms
void PlatformManager::setSpeed(int s)
{
	speed = s;
	
	for (m_it=platforms.begin(); m_it < platforms.end(); m_it++)
		m_it->setSpeed(speed);
}

//Calls move on each platform in platforms vector
void PlatformManager::move(Uint32 deltaTicks)
{	
	for (m_it=platforms.begin(); m_it < platforms.end(); m_it++)
		m_it->move(deltaTicks);
}

//Call show on each platform in platforms vector
void PlatformManager::show(SDL_Surface* pScreen)
{	
	for (m_it=platforms.begin(); m_it < platforms.end(); m_it++)
		m_it->show(pScreen);
}

//Returns true if the object is on a platform
bool PlatformManager::isOnPlatform(GameObject* pObject)
{
	for (m_it=platforms.begin(); m_it < platforms.end(); m_it++)
    {
		if(m_it->onPlatform(pObject)) 
            return true;
	}
	
	return false;
}

//Returns y value of the platform if the object has just gone through the platform, -1 otherwise
float PlatformManager::throughPlatform(GameObject* pObject, Uint32 deltaTicks)
{
	float y = -1;
	
	for (m_it=platforms.begin(); m_it < platforms.end(); m_it++)
    {
		y = m_it->throughPlatform(pObject,deltaTicks);
		if(y >= 0) 
            return y;
	}
	
	return y;
}

//Removes platforms which are not visible
void PlatformManager::clearInvisible()
{	
	m_it=platforms.begin();
    
	while(m_it < platforms.end())
    {
		if(!m_it->isVisible())
			m_it = platforms.erase(m_it);
        
		else
            m_it++;
	}
}