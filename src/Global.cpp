/*
 *  Global.cpp
 *  Climber
 *
 *  Created by Arthur Fox on 17/09/2011.
 *  Copyright 2011 Arthur Fox. All rights reserved.
 *
 */

#include "Global.h"
#include "Path.h"
#include <iostream>
#include <unistd.h>

#pragma mark -
#pragma mark SingletonPattern

Global* Global::ms_pGlobal(0);

Global* Global::sharedGlobal()
{	
	if(ms_pGlobal == 0)
		ms_pGlobal = new Global();
	
	return ms_pGlobal;
}

Global::Global()
{
    initSDL();
    initFontsColors();
    initMusic();
}

#pragma mark -
#pragma mark Initialisation

void Global::initSDL() const
{
    //Initialise all SDL subsystems
	if ( SDL_Init (SDL_INIT_EVERYTHING) < 0 ) {
		printf("Could not initialise SDL: %s\n", SDL_GetError());
		exit(1);
	}
}

void Global::initFontsColors()
{
    
    //Initialise SDL_ttf
	if( TTF_Init() == -1){
		printf("Could not initialise TTF: %s\n", TTF_GetError());
		exit(1);
	}
    
    //NOT QUITE SURE HOW THIS IS NOT DESTROYED BY THE FUNCTION STACK BEING DESTROYED?
    //MAYBE A COPY CONSTRUCTOR IS USED IN m_whiteColour = white ?
    //Init colours
    SDL_Color white = { 255, 255, 255 };
    m_whiteColour = white;
    
    //Load fonts
    std::string font = ms_pPath->pathForFile(RESOURCE_FONT);
    m_pSmallFont  = TTF_OpenFont( font.c_str(), 28 );
    m_pMediumFont = TTF_OpenFont( font.c_str(), 56 );
    m_pLargeFont  = TTF_OpenFont( font.c_str(), 112 );
    if(m_pSmallFont == NULL || m_pMediumFont == NULL || m_pLargeFont == NULL){
		printf("Could not load font: %s\n", TTF_GetError());
		exit(1);
	}
}

void Global::initMusic()
{
    //Initialise SDL_mixer 
    if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) { 
        exit(1);
    }
    
    //Not muted to begin with
    m_muted = false; 
    
    //Initialise music
    std::string music = ms_pPath->pathForFile(RESOURCE_MUSIC);
    m_pMusic = Mix_LoadMUS( music.c_str());
    if(m_pMusic == NULL){
        printf("Could not load music: %s\n", Mix_GetError());
        exit(1);
    }
}

#pragma mark -
#pragma mark FontFunctions

//Return desired font size
TTF_Font* Global::getFont(int font) const
{
    switch (font) 
    {
        case SMALL_FONT:
            return m_pSmallFont;
            break;
            
        case MEDIUM_FONT:
            return m_pMediumFont;
            break;
            
        case LARGE_FONT:
            return m_pLargeFont;
            break;
            
        default:
            return m_pSmallFont;
            break;
    }
}

//Return desired color
SDL_Color Global::getColor(int color) const
{
    switch (color) 
    {
        case WHITE:
            return m_whiteColour;
            break;
            
        default:
            return m_whiteColour;
            break;
    }
}


#pragma mark -
#pragma mark GlobalHelpers

//Returns optimized image when loading
SDL_Surface* Global::loadImage(const std::string& filename) const
{
	SDL_Surface* pLoadedImage = NULL;
	SDL_Surface* pOptimizedImage = NULL;
	
    std::string path = ms_pPath->pathForFile(filename);
    pLoadedImage = IMG_Load( path.c_str() );
    
	if( pLoadedImage != NULL )
	{
		pOptimizedImage = SDL_DisplayFormatAlpha(pLoadedImage);
		SDL_FreeSurface( pLoadedImage );
	}
	else {
		std::cout << "ERROR: Loading image " << filename << " has failed" << std::endl;
	}
	
	return pOptimizedImage;
}

//Blit source image onto destination - clip can be used for sprite sheets
void Global::applySurface(int x, int y, SDL_Surface* pSource, SDL_Surface* pDestination, SDL_Rect* pClip) const
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	
    //LEAKING MEMORY when called from Scene::ApplyInGame()
	SDL_BlitSurface( pSource, pClip, pDestination, &offset );
}