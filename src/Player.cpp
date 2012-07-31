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

// Constants
const int INIT_JUMP = 766;     //jump = 360;
const int INIT_WALK = 736;     //walk = 400;
const int INIT_GRAVITY = 3090; //gravity = 720;
const int FRICTION = 400;

Player::Player(PlatformManager* pPlatManager): GameObject()
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
	m_pPlatManager = pPlatManager;
	m_jump = INIT_JUMP; 
	m_walk = INIT_WALK;     
    m_gravity = INIT_GRAVITY; 
}


//Moves the player taking time elapsed into account
bool Player::move(Uint32 deltaTicks)
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
	
	
	//DEBUG
	//printf("BEFORE: y = %f; \t yVel = %f\n", y, yVel);
	//Move the player UP or DOWN
	m_y += m_yVel * ( deltaTicks / 1000.f );
	
	//Keep increasing the effect of gravity on player
	m_yVel += m_gravity * ( deltaTicks / 1000.f );
	//DEBUG
	//printf("AFTER:  y = %f; \t yVel = %f\n\n", y, yVel);
	
	
    //If player touches the ground return false as its Game Over
	if (m_y >= GROUND_HEIGHT){
		return false;
	}
    
	//Check if falling down, if so and going through platform stay on top of platform
	else if (m_yVel >= 0){
		
        //If player is going through a platform this returns the height of the platform
		float yVelPlat = m_pPlatManager->throughPlatform(this, deltaTicks);
		
        //If player is going through platform set its height accordingly and set its yVel to that of the platform
		if(yVelPlat >= CEILING){
			m_y = yVelPlat;
			m_yVel = m_pPlatManager->getSpeed();
		}
	}
	
	//printf("move() isOnPlatform(): %d\n", pManager->isOnPlatform(this));
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

void Player::handleInput(Uint32 deltaTicks)
{	
	//Get current state of keyboard
	Uint8* pKeystates = SDL_GetKeyState(NULL);
    
    //Check if player is on the platform
    bool onPlatform = m_pPlatManager->isOnPlatform(this);
	
	//Adjust the velocity if a key was pressed
	if(pKeystates[SDLK_LEFT])
    {
        m_xVel = -m_walk;
    }
	else if(pKeystates[SDLK_RIGHT])
    {
		m_xVel = m_walk;
    }
    
    //Keep the momentum going even when player not holding direction key
    /* TODO - Introduce Momentum if I think it is necessary!
     else if (xVel < 0 && movingLeft && !onPlatform)
     {
     xVel += walk*(deltaTicks/1000.f);
     printf("Left: %f, %d\n", xVel, movingLeft);
     }
     else if (xVel > 0 && !movingLeft && !onPlatform)
     {
     xVel -= walk*(deltaTicks/1000.f);
     printf("Right: %f, %d\n", xVel, movingLeft);
     }
     */
    else
    {
        m_xVel = 0;
        //printf("Neither: %f, %d\n", xVel, movingLeft);
    }
    
    
    //printf("handleInput() isOnPlatform(): %d\n", pManager->isOnPlatform(this));
    
    //Add friction to platforms
    if(onPlatform && m_xVel > FRICTION)
        m_xVel = FRICTION;
    else if(onPlatform && m_xVel < -FRICTION)
        m_xVel = -FRICTION;
	
    //Handle player jumps
	if(onPlatform && pKeystates[SDLK_UP] && m_yVel >= 0){
		//printf("JUMPED\n");
		m_yVel = -m_jump;
	}
}

//Return jump
int Player::getJump()
{
	return m_jump;
}

//Set jump
void Player::setJump(int j)
{
	m_jump = j;
}

//Return gravity
int Player::getGravity()
{
	return m_gravity;
}

//Set gravity
void Player::setGravity(int g)
{
	m_gravity = g;
}

//Return walk
int Player::getWalk()
{
	return m_walk;
}

//Set gravity
void Player::setWalk(int w)
{
	m_walk = w;
}