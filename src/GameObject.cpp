/*
 *  GameObject.cpp
 *  Climber
 *
 *  Created by Arthur Fox on 16/09/2011.
 *  Copyright 2011 Arthur Fox. All rights reserved.
 *
 */

#include "GameObject.h"
#include "Global.h"

//Contructor for collision lines
GameObject::CollisionBox::CollisionBox()
{
	x1 = x2 = y1 = y2 = 0;
}

//Draws object on screen if it is within the screen bounds and not NULL
void GameObject::show(SDL_Surface* pScreen)
{
    Global::sharedGlobal()->applySurface( m_x, m_y, m_pImage, pScreen );
}

//Returns the object's x value
float GameObject::getX()
{
	return m_x;
}

//Returns the object's y value
float GameObject::getY()
{
	return m_y;
}

//Returns the object's xVel
float GameObject::getXVel()
{
	return m_xVel;
}

//Returns the object's yVel
float GameObject::getYVel()
{
	return m_yVel;
}

//Returns the height of object or -1 if image not yet given to object
int GameObject::getHeight()
{
	if (m_pImage!=NULL)
		return m_pImage->h;
	else 
		return -1;
}

//Returns the width of object or -1 if image not yet given to object
int GameObject::getWidth()
{
	if (m_pImage!=NULL)
		return m_pImage->w;
	else 
		return -1;
}
