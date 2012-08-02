//
//  Scene.cpp
//  Climber
//
//  Created by Arthur Fox on 05/06/2012.
//  Copyright (c) 2012 OMA. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include "SDL_ttf.h"
#include "Scene.h"

//initialise platform array and speed
Scene::Scene()
{
    m_pGlobal = Global::sharedGlobal();
    
    //Load the background
    m_pBackgroundImage = m_pGlobal->loadImage(RESOURCE_BACKGROUND);
    if(m_pBackgroundImage == NULL ){
		printf("Could not load background image: %s\n", SDL_GetError());
		exit(1);
	}
    
    //Render fps text
	std::stringstream stream;
	stream << FRAMES_PER_SECOND;
	std::string mystr = stream.str();
	m_pFrames = TTF_RenderText_Solid( m_pGlobal->getFont(SMALL_FONT), mystr.c_str(), m_pGlobal->getColor(WHITE) );
	if( m_pFrames == NULL ){
		printf("Could render fps: %s\n", TTF_GetError());
		exit(1);
	}
    
    //Render Climber intro text
	m_pClimberText = TTF_RenderText_Solid( m_pGlobal->getFont(LARGE_FONT), "CLIMBER", m_pGlobal->getColor(WHITE) );
	if( m_pClimberText == NULL ){
		printf("Could not render climber text: %s\n", TTF_GetError());
		exit(1);
	}
    
    //Render the starting message
	m_pStartText = TTF_RenderText_Solid( m_pGlobal->getFont(SMALL_FONT), "Press Enter to begin game", m_pGlobal->getColor(WHITE) );
	if( m_pStartText == NULL ){
		printf("Could not render start text: %s\n", TTF_GetError());
		exit(1);
	};
    
    //Render game over text
	m_pGameOverText = TTF_RenderText_Solid( m_pGlobal->getFont(MEDIUM_FONT), "GAME OVER" , m_pGlobal->getColor(WHITE) );
	if( m_pGameOverText == NULL ){
		printf("Could not render game over: %s\n", TTF_GetError());
		exit(1);
	}
	
	//Render tryAgain text
	m_pTryAgainText = TTF_RenderText_Solid( m_pGlobal->getFont(SMALL_FONT), "Press any key to try again" , m_pGlobal->getColor(WHITE) );
	if( m_pTryAgainText == NULL ){
		printf("Could not render try again: %s\n", TTF_GetError());
		exit(1);
	}
    
    //Render score text
	m_pScore = TTF_RenderText_Solid( m_pGlobal->getFont(SMALL_FONT), "0", m_pGlobal->getColor(WHITE) );
	if( m_pScore == NULL ){
		printf("Could not render score: %s\n", TTF_GetError());
		exit(1);
	}
    
    //Render muted icon text
	m_pMutedIcon = TTF_RenderText_Solid( m_pGlobal->getFont(SMALL_FONT), "M", m_pGlobal->getColor(WHITE) );
	if( m_pMutedIcon == NULL ){
		printf("Could not render mutedIcon: %s\n", TTF_GetError());
		exit(1);
	}
    
    //Load the climber image itself
    m_pPlayer = Global::sharedGlobal()->loadImage(RESOURCE_PLAYER);
    if(m_pPlayer == NULL ){
        printf("Could not load player: %s\n", TTF_GetError());
        exit(1);
    }
}

#pragma mark -
#pragma mark Updates

//--------------------------------------------------------------------------
// Private
//--------------------------------------------------------------------------


//Updates the background speed so it keeps in sync with the platforms
void Scene::updateSpeed(Uint32 deltaTicks, PlatformManager& platManager)
{
    //Scroll background up
    m_bgY += platManager.getSpeed() * (deltaTicks/ 1000.f);
    
    //If the background has gone too far reset the offset 
    if( m_bgY >= LEVEL_HEIGHT ) 
        m_bgY = 0;
}


//Update the frame rate
void Scene::updateFrameRate(float fps)
{
	std::stringstream stream;
	stream << std::setprecision(2) << fps;
	std::string mystr = stream.str();
    
    SDL_FreeSurface(m_pFrames);
	m_pFrames = TTF_RenderText_Solid( m_pGlobal->getFont(SMALL_FONT), mystr.c_str(), m_pGlobal->getColor(WHITE) );
}


//Update the frame score
void Scene::updateScore(int sc)
{
	std::stringstream stream;
	stream << sc;
	std::string mystr = stream.str();
    
    SDL_FreeSurface(m_pScore);
	m_pScore = TTF_RenderText_Solid( m_pGlobal->getFont(SMALL_FONT), mystr.c_str(), m_pGlobal->getColor(WHITE) );
}


//--------------------------------------------------------------------------
// Public
//--------------------------------------------------------------------------

//Update for when the game is actually playing
void Scene::updateInGame(Uint32 deltaTicks, PlatformManager& platManager, float fps, int sc)
{
    updateSpeed(deltaTicks, platManager);
    updateFrameRate(fps);
    updateScore(sc);
}

//Reset the speed when the player loses
void Scene::resetScene()
{
    m_bgX = m_bgY = 0;
    updateFrameRate(0);
    updateScore(0);
}


#pragma mark -
#pragma mark Renders

//--------------------------------------------------------------------------
// Public
//--------------------------------------------------------------------------


// Renders the background in the main menu
void Scene::renderInMainMenu(SDL_Surface* pScreen)
{
    //Place background and Write Climber onto screen
    m_pGlobal->applySurface( 0, 0, m_pBackgroundImage, pScreen);
	m_pGlobal->applySurface( (SCREEN_WIDTH - m_pClimberText->w)/2, (SCREEN_HEIGHT - m_pClimberText->h)/2 - 100, m_pClimberText, pScreen );
	
	//Draw the climber
	m_pGlobal->applySurface( (SCREEN_WIDTH - m_pPlayer->w)/2, (SCREEN_HEIGHT - m_pPlayer->h)/2 + 30, m_pPlayer, pScreen );
	
	//Write for user to press any button to start
	m_pGlobal->applySurface( (SCREEN_WIDTH - m_pStartText->w)/2, (SCREEN_HEIGHT - m_pStartText->h)/2 + 80, m_pStartText, pScreen );
	if( SDL_Flip( pScreen ) == -1 )
		exit(1);
}


// Renders the scrolling background for in-game
void Scene::renderInGame(SDL_Surface* pScreen, float fps, int sc)
{
    //Apply background twice to give illusion of scrolling backgrounds
    m_pGlobal->applySurface( m_bgX, m_bgY, m_pBackgroundImage, pScreen);
    m_pGlobal->applySurface( m_bgX, m_bgY - LEVEL_HEIGHT, m_pBackgroundImage, pScreen);
    
    //Render the frame rate and score
    m_pGlobal->applySurface( 10, 0, m_pFrames, pScreen );
    m_pGlobal->applySurface( SCREEN_WIDTH - m_pScore->w -10, 0, m_pScore, pScreen );
    
    //Render the mute icon if necessary
    if (m_pGlobal->getMuted()) 
        m_pGlobal->applySurface( SCREEN_WIDTH - m_pMutedIcon->w - 12, SCREEN_HEIGHT - 50, m_pMutedIcon, pScreen );
}

// Renders the background in the game over scene
void Scene::renderInGameOver(SDL_Surface* pScreen, int sc)
{
    //Replace with the gameover background and write game over
    m_pGlobal->applySurface( 0, 0, m_pBackgroundImage, pScreen); 
    m_pGlobal->applySurface( (SCREEN_WIDTH - m_pGameOverText->w)/2, (SCREEN_HEIGHT - m_pGameOverText->h)/2 - 50, m_pGameOverText, pScreen );

    //Write the "SCORE: sc" into score variable
    std::stringstream stream;
    stream << "SCORE: " << sc;
    std::string mystr = stream.str();
    
    SDL_FreeSurface(m_pScore);
    m_pScore = TTF_RenderText_Solid( m_pGlobal->getFont(SMALL_FONT), mystr.c_str(), m_pGlobal->getColor(WHITE) );

    //Write score + tryAgain onto background and update the screen
    m_pGlobal->applySurface( (SCREEN_WIDTH - m_pScore->w)/2, (SCREEN_HEIGHT - m_pScore->h)/2 + 35, m_pScore, pScreen );
    m_pGlobal->applySurface( (SCREEN_WIDTH - m_pTryAgainText->w)/2, (SCREEN_HEIGHT - m_pTryAgainText->h)/2 + 80, m_pTryAgainText, pScreen );
    if( SDL_Flip( pScreen ) == -1 )
        exit(1);
}
