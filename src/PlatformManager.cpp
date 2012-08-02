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

//initialise platform array and speed
PlatformManager::PlatformManager()
{
	m_speed = INIT_PLAT_SPEED;
    
	newPlatform(SCREEN_WIDTH/2 - 60 , PLAYER_HEIGHT);
}

//Private function for adding a new platform to the platforms vector 
void PlatformManager::newPlatform(int initialX, int initialY)
{
	Platform plat(m_speed, initialX, initialY);
	platforms.push_back(plat);
}

//Sets speed of all platforms
void PlatformManager::setSpeed(int s)
{
	m_speed = s;
	
	for (m_it=platforms.begin(); m_it < platforms.end(); m_it++)
		m_it->setSpeed(m_speed);
}

//Calls update on each platform in platforms vector
void PlatformManager::update(Uint32 deltaTicks)
{	
	for (m_it=platforms.begin(); m_it < platforms.end(); m_it++)
		m_it->update(deltaTicks);
}

//Call render on each platform in platforms vector
void PlatformManager::render(SDL_Surface* pScreen)
{	
	for (m_it=platforms.begin(); m_it < platforms.end(); m_it++)
		m_it->render(pScreen);
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

//Returns true if the object is on a platform
bool PlatformManager::isOnPlatform(GameObject& rObject)
{
	for (m_it=platforms.begin(); m_it < platforms.end(); m_it++)
    {
		if(m_it->onPlatform(rObject)) 
            return true;
	}
	
	return false;
}

//Returns y value of the platform if the object has just gone through the platform, -1 otherwise
float PlatformManager::throughPlatform(GameObject& rObject, Uint32 deltaTicks)
{
	float y = -1;
	
	for (m_it=platforms.begin(); m_it < platforms.end(); m_it++)
    {
		y = m_it->throughPlatform(rObject, deltaTicks);
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