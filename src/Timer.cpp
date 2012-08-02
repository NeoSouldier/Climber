/*
 *  Timer.cpp
 *  Climber
 *
 *  Created by Arthur Fox on 13/09/2011.
 *  Copyright 2011 Arthur Fox. All rights reserved.
 *
 */

#include "Timer.h"

Timer::Timer()
{
	m_startTicks = 0;
	m_pausedTicks = 0;
	m_paused = false;
	m_started = false;
}

void Timer::start()
{
	m_started = true;
	m_paused = false;
	m_startTicks = SDL_GetTicks();
}

void Timer::stop()
{
	m_started = false;
	m_paused = false;
}

void Timer::pause()
{
	//If the timer is running and isn't already paused
	if( ( m_started == true ) && ( m_paused == false ) )
	{		
		//Pause and Calculate the paused ticks
		m_paused = true;
		m_pausedTicks = SDL_GetTicks() - m_startTicks;
	}
}

void Timer::unpause()
{
	//If the timer is paused
	if( m_paused == true )
	{
		//Unpause the timer and reset the starting ticks
		m_paused = false;
		m_startTicks = SDL_GetTicks() - m_pausedTicks;
		m_pausedTicks = 0;
	}
}

int Timer::getTicks() const
{
	//If the timer is running
	if( m_started == true )
	{
		if( m_paused == true )
			return m_pausedTicks;
		else
			return SDL_GetTicks() - m_startTicks;
	}
	
	//If the timer isn't running
	return 0;
}