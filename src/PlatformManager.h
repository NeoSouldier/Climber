/*
 *  PlatformManager.h
 *  Climber
 *
 *  Created by Arthur Fox on 17/09/2011.
 *  Copyright 2011 Arthur Fox. All rights reserved.
 *
 */

#ifndef Climber_PlatformManager_h
#define Climber_PlatformManager_h

#include "SDL.h"
#include <vector>
#include "Platform.h"
#include "GameObject.h"

//Ugly Forward Declaration
//Could possibly avoid this if Platforms took care of their own image loading!
class Platform;

class PlatformManager
{
private:	
	//Vector of platforms
	std::vector<Platform> platforms;
    
    //Iterator for platforms
    std::vector<Platform>::iterator m_it;
	
	//Platforms' speed
	int speed;
    
    //Images for the three different platform types
    SDL_Surface *m_pSmall, *m_pMedium, *m_pLarge;
	
	//Creates new platform and adds to platform vector
	void newPlatform(int initialX = -1, int initialY = -1);
	
public:
	//Constructor
	PlatformManager();
    
    //Destructor get rid of all the unnecessary images
	~PlatformManager()
    {
        SDL_FreeSurface(m_pSmall);
        SDL_FreeSurface(m_pMedium);
        SDL_FreeSurface(m_pLarge);
    };

    //Returns the correct image for the desired platform
    SDL_Surface* getPlatformImage(int platform);
    
	//Adds platform to platforms vector, ensuring 100 pixel restriction, returns true if platform added, false otherwise
	bool addPlatform(int initialX = -1, int initialY = -1);
	
	//get and set current speed of platforms
	int getSpeed();
	void setSpeed(int s);
    
	//Move platforms
	void move(Uint32 delta);
	
	//Show platforms
	void show(SDL_Surface* pScreen);
	
	//Returns true if object is on a platform
	bool isOnPlatform(GameObject* pObject);
	
	//If object is going through a platform it returns y value of the specific platform, -1 otherwise
	float throughPlatform(GameObject* pOject, Uint32 deltaTicks);
	
	//Clear platforms from vector if not visible
	void clearInvisible();
	
};

#endif