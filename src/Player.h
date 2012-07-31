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
	int m_jump, m_walk, m_gravity;
	
public:
	//Initialises the variables
	Player(PlatformManager* manager);
    
    //Destructor
    ~Player()
    {
        SDL_FreeSurface(m_pImage);
    }
    
    //Moves the player, if he is on the ground return false, otherwise true
	bool virtual move(Uint32 deltaTicks);
	
	//Returns the collision box for the player
	const virtual CollisionBox collisionBox(Uint32 deltaTicks = 0);
	
    //Takes key presses and adjusts the player's velocity
	void handleInput(Uint32 deltaTicks);
    
	//Get and Set for jump
	int getJump();
	void setJump(int j);
	
	//Get and Set for gravity
	int getGravity();
	void setGravity(int g);
	
	//Get and Set for walk
	int getWalk();
	void setWalk(int w);
	
};

#endif