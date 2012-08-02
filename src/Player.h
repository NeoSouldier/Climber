/*
 *  Player.h
 *  Climber
 *
 *  Created by Arthur Fox on 07/09/2011.
 *  Copyright 2011 Arthur Fox. All rights reserved.
 *
 */

#ifndef Climber_Player_h
#define Climber_Player_h

#include "GameObject.h"
#include "PlatformManager.h"

class Player : public GameObject
{	
private:
	//Platform Manager for the game this player is in
	PlatformManager* m_pPlatManager;
	
	//How many pixels the player can jump, walk and gravity imposed on him all in terms of pixels
	int m_jump, m_gravity, m_walk;
	
public:
	//Initialises the variables
	Player(PlatformManager& rPlatManager);
    
    //Destructor
    ~Player()
    {
        SDL_FreeSurface(m_pImage);
    }
    
    //Takes key presses and adjusts the player's velocity
	void handleInput(Uint32 deltaTicks);
    
    //Moves the player, if he is on the ground return false, otherwise true
	bool virtual update(Uint32 deltaTicks);
	
	//Returns the collision box for the player
	const virtual CollisionBox collisionBox(Uint32 deltaTicks = 0);
    
	//Get and Set for jump
	inline int getJump() const  { return m_jump;}
	inline void setJump(int j)  { m_jump = j;   }
	
	//Get and Set for gravity
    inline int getGravity() const { return m_gravity;}  
	inline void setGravity(int g) { m_gravity = g;   }
	
	//Get and Set for walk
	inline int getWalk() const { return m_walk;}  
	inline void setWalk(int w) { m_walk = w;   }
	
};

#endif