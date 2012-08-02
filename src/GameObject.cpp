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
void GameObject::render(SDL_Surface* pScreen) const
{
    Global::sharedGlobal()->applySurface( m_x, m_y, m_pImage, pScreen );
}