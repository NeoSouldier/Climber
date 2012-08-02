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

class PlatformManager
{
private:	
	//Vector of platforms
	std::vector<Platform> platforms;
    
    //Iterator for platforms
    std::vector<Platform>::iterator m_it;
	
	//Platforms' speed
	int m_speed;
	
	//Creates new platform and adds to platform vector
	void newPlatform(int initialX = -1, int initialY = -1);
	
public:
	//Constructor
	PlatformManager();
	
	//get and set current speed of platforms
	inline int getSpeed() const {return m_speed;}
    void setSpeed(int s);
    
	//Move platforms
	void update(Uint32 delta);
	
	//Show platforms - Not const because it changes m_it
	void render(SDL_Surface* pScreen);
    
    //Adds platform to platforms vector, ensuring 100 pixel restriction, returns true if platform added, false otherwise
	bool addPlatform(int initialX = -1, int initialY = -1);
    
    //Returns true if object is on a platform
	bool isOnPlatform(GameObject& rObject);
	
	//If object is going through a platform it returns y value of the specific platform, -1 otherwise
	float throughPlatform(GameObject& rOject, Uint32 deltaTicks);
    
    //Clear platforms from vector if not visible
	void clearInvisible();
	
};

#endif