/*
 *  GameObject.h
 *  Climber
 *
 *  Created by Arthur Fox on 16/09/2011.
 *  Copyright 2011 Arthur Fox. All rights reserved.
 *
 */

#ifndef Climber_GameObject_h
#define Climber_GameObject_h

#include "SDL.h"

class CollisionLine;

//This class contains variables and functions that all game objects must contain
//NOTE: All subclasses must initialise EVERY protected variable in their constructor
class GameObject
{
protected:
	//The X and Y offsets of the object
	float m_x, m_y;
	
	//The velocity of the object
	float m_xVel, m_yVel;
	
	//Image of the object
	SDL_Surface* m_pImage;

public:

    //Constructor just for good practice
    GameObject()
    {
        m_pImage = NULL;
    }
	
    //Virtual destructor to avoid memory leaks when classes inheriting from GameObject are destroyed
    virtual ~GameObject(){}
    
	//Nested class used for collision detection 
	//NOTE: This is an Abstract concept the box is not necessarily the outline of the image
	class CollisionBox
	{
	public:
		//Line variables
		float x1, x2, y1, y2;
		
		//Constructor
		CollisionBox();
	};
	
	//Returns the object's collision line - uses deltaTicks to calculate y1 and MUST be implemented by object
	const CollisionBox virtual collisionBox(Uint32 deltaTicks = 0) = 0;
	
	//Moves the object, returns true on success - MUST be implemented by actual object 
	bool virtual move(Uint32 deltaTicks) = 0;
	
	//Shows the object on the screen
	void show(SDL_Surface* pScreen);
    
    //Sets the camera over the dot 
    void setCamera();
	
	//Returns dimensions of object
	//NOTE: Height and Width return -1 if the image failed to load
	float getX();
	float getY();
	float getXVel();
	float getYVel();
	int getHeight();
	int getWidth();
};

#endif