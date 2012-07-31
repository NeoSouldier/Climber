//
//  Scene.h
//  Climber
//
//  Created by Arthur Fox on 05/06/2012.
//  Copyright (c) 2012 OMA. All rights reserved.
//

#ifndef Climber_Background_h
#define Climber_Background_h

#include "PlatformManager.h"
#include "Global.h"

class Scene
{
private:
    
    //Global pointer
    Global* m_pGlobal;
    
    //The offsets of the background 
    float m_bgX, m_bgY;
	
	//Surfaces
	SDL_Surface *m_pBackgroundImage, *m_pFrames, *m_pScore, *m_pMutedIcon, 
                *m_pPlayer, *m_pClimberText, *m_pStartText, *m_pGameOverText, 
                *m_pTryAgainText;
    
    //Private update functions
    void updateSpeed(Uint32 deltaTicks, PlatformManager& platManager);
    void updateFrameRate(float fps);
    void updateScore(int sc);
    
public:
    
    //Constructor
    Scene();
    
    //Update functions for when in game
    void updateInGame(Uint32 deltaTicks, PlatformManager& pManager, float fps, int sc);
    void resetScene();
    
    //Apply methods for rendering in different parts of the game
    void applyInMainMenu(SDL_Surface* pScreen);
    void applyInGame(SDL_Surface* pScreen, float fps, int sc); 
    void applyInGameOver(SDL_Surface* pScreen, int sc);
};

#endif
