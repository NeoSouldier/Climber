/*
 *  Player.cpp
 *  Climber
 *
 *  Created by Arthur Fox on 07/09/2011.
 *  Copyright 2011 Arthur Fox. All rights reserved.
 *
 */

#include "Player.h"
#include "Global.h"
#include "GameDirector.h"

/* INFO ABOUT THE PLAYER:
 
 const int ARM_LENGTH = 35;
 const int DISTANCE_BETWEEN_LEGS = 40;
 
 ie.
 
 arm - body - arm
     leg-leg
 ----|------|----
  35    40    35
 
 = 110
 
 */

Player::Player(PlatformManager& rPlatManager)
{	
    
	//Initialise the player offsets
	m_x = (SCREEN_WIDTH - PLAYER_WIDTH)/2;
	m_y = CEILING;
	
	//Initialise the velocity
	m_xVel = 0;
	m_yVel = 0;
	
	//Load player image
	m_pImage = Global::sharedGlobal()->loadImage(RESOURCE_PLAYER);
	
	//Save pointer to Platform Manager and set jump height
	m_pPlatManager = &rPlatManager;
	m_jump = INIT_JUMP; 
	m_walk = INIT_WALK;     
    m_gravity = INIT_GRAVITY; 
}

//Handle the input accordingly so that the update can perform correctly
void Player::handleInput(Uint32 deltaTicks)
{	
	//Get current state of keyboard
	Uint8* pKeystates = SDL_GetKeyState(NULL);
    
    //Check if player is on the platform
    bool onPlatform = m_pPlatManager->isOnPlatform(*this);
	
	//Adjust the velocity if a key was pressed
	if(pKeystates[SDLK_LEFT])
        m_xVel = -m_walk;
	else if(pKeystates[SDLK_RIGHT])
		m_xVel = m_walk;
    else
        m_xVel = 0;
    
    //Add friction to platforms
    if(onPlatform && m_xVel > FRICTION)
        m_xVel = FRICTION;
    else if(onPlatform && m_xVel < -FRICTION)
        m_xVel = -FRICTION;
	
    //Handle player jumps
	if(onPlatform && pKeystates[SDLK_UP] && m_yVel >= 0)
		m_yVel = -m_jump;
}

//Updates the player's state taking time elapsed into account
bool Player::update(Uint32 deltaTicks)
{
	//Move the player LEFT or RIGHT 
	m_x += m_xVel * ( deltaTicks / 1000.f );
	
	//If the player went too far to the left or right move back
	if( m_x < LEFT_WALL){
		m_x = LEFT_WALL;
		m_xVel = 0;
	}
	else if ( m_x > RIGHT_WALL - PLAYER_WIDTH ){
		m_x = RIGHT_WALL - PLAYER_WIDTH;
		m_xVel = 0;
	}
	
    //Move the player UP or DOWN
	m_y += m_yVel * ( deltaTicks / 1000.f );
	
	//Keep increasing the effect of gravity on player
	m_yVel += m_gravity * ( deltaTicks / 1000.f );
	
    //If player touches the ground return false as its Game Over
	if (m_y >= GROUND_HEIGHT){
		return false;
	}
    
	//Check if falling down, if so and going through platform stay on top of platform
	else if (m_yVel >= 0){
		
        //If player is going through a platform this returns the height of the platform
		float yVelPlat = m_pPlatManager->throughPlatform(*this, deltaTicks);
		
        //If player is going through platform set its height accordingly and set its yVel to that of the platform
		if(yVelPlat >= CEILING){
			m_y = yVelPlat;
			m_yVel = m_pPlatManager->getSpeed();
		}
	}
	
	return true;	
}


//Method which creates a collision box for the player, if deltaTicks -1 Time is not taken into account
const GameObject::CollisionBox Player::collisionBox(Uint32 deltaTicks)
{
	CollisionBox col = CollisionBox();
	col.x1 = m_x + 35;																		// LEFT FOOT
	col.x2 = m_x + 75;																		// RIGHT FOOT
	col.y2 = m_y + this->getHeight();                                                         // BOTTOM PART
	
	//If we are dealing with Time then the bottom has moved, otherwise set it to what it currently is
	if(deltaTicks > 0)
		col.y1 = (m_y + this->getHeight()) - m_yVel * ( deltaTicks / 1000.f );                  // TOP PART = BOTTOM PART BEFORE MOVING 
	else 
		col.y1 = col.y2;																	  //BOTTOM PART
	
	return col;
}