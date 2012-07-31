/*
 *  Timer.h
 *  Climber
 *
 *  Created by Arthur Fox on 13/09/2011.
 *  Copyright 2011 Arthur Fox. All rights reserved.
 *
 */

#ifndef Climber_Timer_h
#define Climber_Timer_h

#include "SDL.h"

class Timer
{
private:
	//The clock time when the timer started, and stored when paused
	int m_startTicks, m_pausedTicks;
	
	//The timer status
	bool m_started, m_paused;
	
public:
	//Initializes variables
	Timer();
	
	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();
	
	//Gets the timer's time
	int getTicks();
	
	//Checks the status of the timer
	bool isStarted();
	bool isPaused();
};

#endif