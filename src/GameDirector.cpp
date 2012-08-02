///*
// *  GameDirector.cpp
// *  Climber
// *
// *  Created by Arthur Fox on 07/09/2011.
// *  Copyright 2011 Arthur Fox. All rights reserved.
// *
// */
//

#include "GameDirector.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

//Statics need their memory initialised in cpp files
GameDirector* GameDirector::ms_pDirector(0);
Global* GameDirector::ms_pGlobal(0);

#pragma mark -
#pragma mark SingletonPattern

GameDirector* GameDirector::sharedDirector()
{	
	if(ms_pDirector == 0)
		ms_pDirector = new GameDirector();
	
	return ms_pDirector;
}

GameDirector::GameDirector()
{
    ms_pGlobal = Global::sharedGlobal();
    
    init();
    m_pScene = new Scene();
    
    m_fps = FRAMES_PER_SECOND;
	resetSeconds();
}

#pragma mark -
#pragma mark InitFunctions

void GameDirector::init()
{
	
	//Initialise the screen 
	SDL_WM_SetCaption( "Climber", NULL );
	m_pScreen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
	if (m_pScreen == NULL) {
		printf("Could not set 640x480x%d video mode: %s\n", SCREEN_BPP, SDL_GetError());
		SDL_Quit();
		exit(1);
	}
    
    //Render pause text
	m_pPause = TTF_RenderText_Solid( ms_pGlobal->getFont(SMALL_FONT), "PAUSED", ms_pGlobal->getColor(WHITE) );
	if( m_pPause == NULL ){
		printf("Could not render pause: %s\n", TTF_GetError());
		exit(1);
	}
}

#pragma mark -
#pragma mark GameFunctions

// Game related functions
void GameDirector::gameLoop()
{	
	//Main loop's boolean variable, while this is true the game should still run
	bool game = true;
	
	//Main while loop
	while(game){
		
		//Main Menu screen
		game = mainMenu();
		
		while(game){
			//Plays the game, returning whether the game should be exited or not
			game = playingLoop();
			
			//Returns false if user Xs out, otherwise loop through game again
			if(game)
				game = gameOver();
		}
	}
}

// STATIC Event filter for main menu- filter so that only enter and quit can pass through
int GameDirector::mainmenuEventFilter(const SDL_Event *pEvent)
{
	if( (pEvent->type == SDL_KEYDOWN  && pEvent->key.keysym.sym == SDLK_RETURN)
		 || pEvent->type == SDL_QUIT)
		return 1;
	
	return 0;
}

// STATIC Event filter for pausing game - filter so that any keypress and quit can pass through
int GameDirector::gameoverEventFilter(const SDL_Event *pEvent)
{
	if( pEvent->type == SDL_KEYDOWN || pEvent->type == SDL_QUIT)
		return 1;
	
	return 0;
}


// STATIC Event filter for pausing game - filter so that only unpause and quit can pass through
int GameDirector::pauseEventFilter(const SDL_Event *pEvent)
{
	if( (pEvent->type == SDL_KEYDOWN && pEvent->key.keysym.sym == SDLK_SPACE)
       || pEvent->type == SDL_QUIT)
		return 1;
	
    if(pEvent->type == SDL_KEYDOWN && pEvent->key.keysym.sym == SDLK_m)
    {
        if (ms_pGlobal->getMuted()) 
        {
            Mix_FadeInMusic(ms_pGlobal->getMusic(), -1, 2000 );
            Mix_VolumeMusic(PAUSE_VOLUME);
        }
        else Mix_HaltMusic();
        ms_pGlobal->muteUnMute();   
    }
    
    
	return 0;
}


//Playing loop, returns false is user has X'd out otherwise returns true
bool GameDirector::playingLoop()
{
	//Return value
	bool game = true;
	
	/***** VARIABLES ****/
	
	//Reset score
	m_pScene->resetScene();
	
	//Initialising game loop variables
	bool playing = true;
	
	//Platform manager
	PlatformManager platManager;
	
	//Player
	Player player(platManager);
	
	//Time related variables
	Timer frame, delta;
    int dtAccumulator = 0; //Used for keeping the game updating at discrete FIXED_TIME_STEP's
	double delay = 0;
	int frameSkip = 0;
	
	//Keeps track of time since last move() function call - for frame independent movement
	delta.start();
    
    //Start playing the music
    if (!ms_pGlobal->getMuted()) Mix_FadeInMusic( ms_pGlobal->getMusic(), -1, 8000 );
    
	/** LOOP **/
	
	//Actual Climber game while loop
	while(playing)
	{
		//Start the frame timer
		frame.start();
		
		/****** EVENTS ******/ 
		
		//Empty the queue to guarantee no lag in input! - COULD USE EVENT FILTER HERE!
		//If user hits space pause the game or if user X's out break
		while (SDL_PollEvent(&m_event))
		{
			if(m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_SPACE)
            {
				delta.pause();
				frame.pause();
				game = playing = pauseGame();
				frame.unpause();
				delta.unpause();
			}
            
			else if( m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_m)
            {
				if (ms_pGlobal->getMuted()) Mix_FadeInMusic( ms_pGlobal->getMusic(), -1, 2000 );
                else Mix_HaltMusic();
                ms_pGlobal->muteUnMute();          
			}
            
			else if( m_event.type == SDL_QUIT )
            {
				game = playing = false;
			}
		}
		
		/***** LOGIC *******/
        
        //If the update function returns false the player has died and its GAMEOVER
        if (!update(player, platManager, delta.getTicks(), dtAccumulator))
            break;
		
        //Restart delta timer
		delta.start(); 
        
		/****** RENDERING *******/
		
        //Render as necessary
        render(player, platManager);
		
		/***** HOUSEKEEPING *****/
        
		//If we need to cap the frame rate sleep the remaining frame time 
		//NOTE: unnecessary at 60fps due to SDL_Flip() and VSYNC ensuring it stays at 60fps
		if( frame.getTicks() < 1000 / FRAMES_PER_SECOND){
			SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - frame.getTicks() );
		}
		
		//For keeping the fps counter up to date
		delay += frame.getTicks();
		
		//Once per second block
		if( ++frameSkip >= FRAMES_PER_SECOND )
		{
			oncePerSecond(delay, platManager, player);
			frameSkip = delay = 0;
		}
	}
	
	return game;
}

//Updates game logic with a fixed-time step, returns false if its GAMEOVER
bool GameDirector::update(Player& rPlayer, PlatformManager& rPlatManager, int dt, int& dtAccumulator)
{
    bool result = true;
    
    //Loop through updating as many times as required, will only update at most LIMIT_UPDATES times
    int updates = 0;
    for(; dt >= FIXED_TIME_STEP && updates < LIMIT_UPDATES && result; dt -= FIXED_TIME_STEP, updates++)
        result = updateActual(rPlayer, rPlatManager);        
    
    //Accumulate remaining ticks left between frames
    //if the game froze for some reason dtAccumulator = 0 so we don't "play catch up" in the next few frames
    if (updates < LIMIT_UPDATES)
        dtAccumulator += dt;
    else
        dtAccumulator = 0;
    
    //Once this has accumulated past a FIXED_TIME_STEP, then update the game logic once again
    if (dtAccumulator >= FIXED_TIME_STEP && result)
    {
        result = updateActual(rPlayer, rPlatManager);        
        dtAccumulator -= FIXED_TIME_STEP;
    }
        
    return result;
}

//Actually updates the game logic
bool GameDirector::updateActual(Player& rPlayer, PlatformManager& rPlatManager)
{
    bool result = true;
    
    //Handle player events for the player
    rPlayer.handleInput(FIXED_TIME_STEP);
    
    //Update the background speed
    m_pScene->updateInGame(FIXED_TIME_STEP, rPlatManager, m_fps, m_seconds);
    
    //Move the platform(s) and add platform if necessary
    rPlatManager.update(FIXED_TIME_STEP);
    rPlatManager.addPlatform();
    
    //Move the player, if this returns false he has landed on the floor and its GAMEOVER
    if(!rPlayer.update(FIXED_TIME_STEP))
        result = false;
    
    return result;;
}

//Renders all the assets as necessary, currently return value is just ignored
bool GameDirector::render(Player& rPlayer, PlatformManager& rPlatManager)
{
    bool result = true;
    
    //Apply the background, fps, score and whether we are muted or not
    m_pScene->renderInGame(m_pScreen, m_fps, m_seconds);
    
    //Show the platform(s) on the screen
    rPlatManager.render(m_pScreen);
    
    //Show the player on the screen
    rPlayer.render(m_pScreen);
    
    //NOTE: Flipping the screen reduces the time to 60fps automatically due to VSYNC!
    if( SDL_Flip( m_pScreen ) == -1 )
        exit(1);
    
    return result;
}


//Main menu screen, returns false if user has X'd out otherwise returns true
bool GameDirector::mainMenu()
{
	//Return value
	bool game = true;
	
    //Apply the main menu scene
    m_pScene->renderInMainMenu(m_pScreen);
	
	//Set mainmenuEventFilter
	SDL_SetEventFilter(mainmenuEventFilter);
	
	//Wait for an event to pass through, if its quit then exit otherwise reset data
	if(SDL_WaitEvent(&m_event))
	{
		if( m_event.type == SDL_QUIT )
			game = false;
	}
	
	//Remove the gameoverEventFilter
	SDL_SetEventFilter(NULL);
	
	return game;
}


//Pause the game, returns false if user Xs out
bool GameDirector::pauseGame()
{	
	//Variable for whether game will quit or not
	bool play = true;
    
    //Lower volume if not muted
    int volume = Mix_VolumeMusic(-1);
    if (!ms_pGlobal->getMuted()) Mix_VolumeMusic(PAUSE_VOLUME);
	
	//Write pause on the screen
	ms_pGlobal->applySurface( (SCREEN_WIDTH - m_pPause->w)/2, (SCREEN_HEIGHT - m_pPause->h)/2, m_pPause, m_pScreen );
	if( SDL_Flip( m_pScreen ) == -1 )
		exit(1);
	
	
	//Set pauseEventFilter
	SDL_SetEventFilter(pauseEventFilter);
	
	//Wait for an event to pass through, if its a quit event then quit = true
	//Otherwise it will be an unpause, so ignore it
	if(SDL_WaitEvent(&m_event))
	{
		if( m_event.type == SDL_QUIT )
			play = false;
	}
	
	//Remove the pauseEventFilter
	SDL_SetEventFilter(NULL);
	
    //Reset the volume and return game status
    if (play && !ms_pGlobal->getMuted()) Mix_VolumeMusic(volume);
	return play;
}


//Game over screen, returns false if user Xs out
bool GameDirector::gameOver()
{
	//Variable for whether game will quit or not
	bool play = true;
    
    Mix_FadeOutMusic( 2000 );
	
    //Apply the game over scene
    m_pScene->renderInGameOver(m_pScreen, m_seconds);
	
	//Set gameoverEventFilter
	SDL_SetEventFilter(gameoverEventFilter);
	
    //Sleep for a bit just for people to see their scores
    sleep(1);
    
	//Wait for an event to pass through, if its quit then exit otherwise reset data
	if(SDL_WaitEvent(&m_event))
	{
		if( m_event.type == SDL_QUIT )
			play = false;
		else {
			resetSeconds();
		}
	}
	
	//Remove the gameoverEventFilter
	SDL_SetEventFilter(NULL);
	
	return play;
}

//Per second block
void GameDirector::oncePerSecond(float delay, PlatformManager& rPlatManager, Player& rPlayer)
{	
	//Adjust falling platforms
	rPlatManager.clearInvisible();

	//Adjust score and fps counters
    m_seconds++;
	m_fps = (FRAMES_PER_SECOND*1000.0f)/delay;

	/* DEBUG */
    /*
	while(m_seconds < 50){
		pPlayer->setWalk(pPlayer->getWalk()*1.08);
        pPlayer->setJump(pPlayer->getJump()*1.1);
        pPlayer->setGravity(pPlayer->getGravity()*1.2);
        pPlatManager->setSpeed(pPlatManager->getSpeed()*1.1); 
        printf("TIME: %d - Speed: %d, Jump: %d, Gravity: %d, Walk: %d\n", m_seconds, 
                    pPlatManager->getSpeed(), pPlayer->getJump(),pPlayer->getGravity(), pPlayer->getWalk());
        m_seconds+=10;
    }
    */
    /* DEBUG */
    
    //After every 10 seconds speed up the game, upper limit of 8 multiplying
    if((m_seconds % 10) == 0 && m_seconds <= 80 )
    {	
        rPlayer.setWalk(rPlayer.getWalk()*1.08);
        rPlayer.setJump(rPlayer.getJump()*1.1);
        rPlayer.setGravity(rPlayer.getGravity()*1.2);
        rPlatManager.setSpeed(rPlatManager.getSpeed()*1.1); 
        printf("TIME: %d - Speed: %d, Jump: %d, Gravity: %d, Walk: %d\n", 
                    m_seconds, rPlatManager.getSpeed(), rPlayer.getJump(),rPlayer.getGravity(), rPlayer.getWalk());
	}
}

void GameDirector::cleanUp()
{
	//Free the surfaces
	SDL_FreeSurface( m_pScreen );
	SDL_FreeSurface( m_pPause );
	
	//Quit TTF, Mixer and SDL
    Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}
