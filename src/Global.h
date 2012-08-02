/*
 *  Global.h
 *  Climber
 *
 *  Created by Arthur Fox on 17/09/2011.
 *  Copyright 2011 Arthur Fox. All rights reserved.
 *
 */


#ifndef Climber_Globals_h
#define Climber_Globals_h

#include "SDL.h"
#include <string>
#include "SDL_image.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "Path.h"

#define RESOURCE_PATH           "Climber.app/Contents/Resources/"

#define RESOURCE_FONT            "FuturaMedium.ttf"
#define RESOURCE_MUSIC           "Climbing.wav"
#define RESOURCE_PLAYER          "Player.png"
#define RESOURCE_BACKGROUND      "NewBackground5.png"
#define RESOURCE_SMALL_PLATFORM  "platform96.png"
#define RESOURCE_MEDIUM_PLATFORM "platform128.png"
#define RESOURCE_LARGE_PLATFORM  "platform160.png"

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const int FRAMES_PER_SECOND = 60;

//Update attributes
const int UPDATES_PER_SECOND = 120;
const int LIMIT_UPDATES = 4;  
const float FIXED_TIME_STEP = 1000.0f/UPDATES_PER_SECOND;

//The dimensions of the level 
const int LEVEL_WIDTH = 640; 
const int LEVEL_HEIGHT = 1368;

//Dimensions of player
const int PLAYER_WIDTH = 110;
const int PLAYER_HEIGHT = 80;

//Background attributes
const int CEILING = -100;
const int FLOOR = SCREEN_HEIGHT;
const int WALL_WIDTH = 50;
const int LEFT_WALL = WALL_WIDTH;
const int RIGHT_WALL = SCREEN_WIDTH - WALL_WIDTH;
const int GROUND_HEIGHT = FLOOR - PLAYER_HEIGHT;

//Music attributes
const int DEFAULT_VOLUME = 128;
const int PAUSE_VOLUME = 30;

//Font attributes
const int SMALL_FONT = 1;
const int MEDIUM_FONT = 2;
const int LARGE_FONT = 3;

//Color attributes
const int WHITE = 1;

// Player Constants
const int INIT_JUMP = 766;     //jump = 360;
const int INIT_WALK = 736;     //walk = 400;
const int INIT_GRAVITY = 3090; //gravity = 720;
const int FRICTION = 400;

//Platform attributes
const int DISTANCE_BETWEEN_PLATFORMS = 100;
const int INIT_PLAT_SPEED = 124;

//Platform types
const int SMALL_PLATFORM = 1;
const int MEDIUM_PLATFORM = 2;
const int LARGE_PLATFORM = 3;

class Global 
{
private:
	//Singleton instance
	static Global* ms_pGlobal;
    static Path* ms_pPath;
    
    //Global Variables
    TTF_Font  *m_pSmallFont, *m_pMediumFont, *m_pLargeFont;
    SDL_Color m_whiteColour;
    Mix_Music *m_pMusic;
    bool m_muted;
    
	//Constructor
	Global();
    
    //Initialisations
    void initSDL() const;
    void initFontsColors();
    void initMusic();
	
public:	
	//Singleton pattern
	static Global* sharedGlobal();
	
    //Font related functions
    TTF_Font* getFont(int font) const;
    SDL_Color getColor(int color) const;
    
    //Music + volume related functions
    inline Mix_Music* getMusic() const  {return m_pMusic;}
    inline bool getMuted() const        {return m_muted;}
    inline void muteUnMute()            {m_muted = !m_muted;}
    
	//Global helper functions:
	SDL_Surface* loadImage(const std::string& filename) const;
	void applySurface(int x, int y, SDL_Surface* pSource, SDL_Surface* pDestination, SDL_Rect* clip = NULL) const;
};

#endif