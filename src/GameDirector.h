/*
 *  GameDirector.h
 *  Climber
 *
 *  Created by Arthur Fox on 07/09/2011.
 *  Copyright 2011 Arthur Fox. All rights reserved.
 *
 */

#ifndef Climber_GameDirector_h
#define Climber_GameDirector_h

#include "SDL.h"
#include "SDL_image.h"
#include "PlatformManager.h"
#include "Player.h"
#include "Global.h"
#include "Scene.h"

class GameDirector 
{

private:
    
    //Global and Scene pointers
    static Global* ms_pGlobal; //Needs to be static because of the Event Filters
    Scene* m_pScene;
    
    //Event structure used throughout
    SDL_Event m_event;
	
	//Surfaces to be used
	SDL_Surface *m_pScreen, *m_pPause;
	
	//Seconds alive and frame rate
	int m_seconds;  //This also represents the score
	float m_fps;
	
	//Singleton instance of game director
	static GameDirector* ms_pDirector;
	
	//Constructor - private as its a Singleton
	GameDirector();
    void init();
    
    //Main Game functions
    bool mainMenu();
	bool playingLoop();
	bool pauseGame();
	bool gameOver();
    void oncePerSecond(float delay, PlatformManager* pPlatManager, Player* pPlayer);
    inline void resetSeconds() {m_seconds = 0;}

    //Event Filters
    static int mainmenuEventFilter(const SDL_Event *pEvent);
    static int gameoverEventFilter(const SDL_Event *pEvent);
    static int pauseEventFilter(const SDL_Event *pEvent);
    
public:
    
	//Singleton pattern
	static GameDirector* sharedDirector();

	//In game functions
	void gameLoop();
    void cleanUp();
};

#endif
