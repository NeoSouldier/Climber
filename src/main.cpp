
/* Climber 
	 main.cpp
*/

/*
 TODO:
 
 - Should Platforms look after loading their own images statically? It would remove the Cyclic Forward Declarations between Platform and PlatformManager...
 - Compile on different platforms!
 
 FOR CLIMBER0.3:
 - Rewrite for iPhone using the cocos2d-x framework
 - Perfect calibrations further and add momentum?
 
 /------------------------------------/
 
 CALIBRATION:
 
 Variables that effect game speed: 
 PlatformManager.cpp: speed;
 Player.cpp: walk, jump, gravity;
 
 FOR NOW I HAVE SETTLED ON:
                s  :  j  :  g  : w
 initial		60 : 360 : 720 : 400
 multiply		1.1: 1.1 : 1.2 : 1.08
 
 Limited to multiplying 15 times!
 
 ---------------
 Jump: Gravity
 
 100 : 50
 -> gives a jump of 100 pixels in 2 seconds
 
 100 : 100
 -> gives a jump of 50 pixels in a second
 
 100 : 200 
 -> gives a jump of 25 pixels in 1/2 seconds
 
 100 : 300
 -> gives a jump of 17 in 1/3 seconds
 
 Conclusion (DEFINITELY CORRECT):
 - Time taken to reach peak = j/g eg. 100/200 = 1/2 seconds
 - Peak height if (j<g) = j/g * j/2 pix

 ---------------
 
 NOT VERY GOOD STYLE OF GAME
 For playing game where player speeds up enough he can jump past blocks
 Good Ratios: speed: jump: gravity
							  5  :  30 :  50
                              5  :  30 :  60  (best i think)
 
 ---------------
 
 BETTER STYLE OF GAME
 For playing game where player must jump on every block
 
 The key here is to have: 100 <= [platform pixels down (speed)] + [player pixels up(j:g)]  <= 150
 
 ----------------
 
 (j:g) : speed
 if speed is going down faster than player going up OBVIOUSLY player will die!!!
 
 speed is dependent on (j:g) because greater actual ratio means less seconds pass, ie. speed down is divided by a larger number
 ie. if (j:g) = 1:2 then speed/2 but if it is 1:3 then speed/3!
 
 OK so atm it seems that necessary conditions are that:
		(1)		(j:g) jump > speed down [taking the (j:g) division into account]
		(2)		100 <(j:g) jump + speed down < 140 [or round about]
 
 ITS LOOKING LIKE FOR (2): 125 IS A GOOD VALUE - UNSURE
 ----------------
 
 (walk seems irrelevant, just keep it at 400)
 
 NOT RIGHT YET        |		speed: jump: gravity
 BUT WORKS PRETTY WELL| (1) 5  :  30 :   60   -> (50/2) + (300/4) = 25 + 75 = 100 - BEST SO FAR
											| (2) 6  :  36 :   72   -> (60/2) + (360/4) = 30 + 90 = 120 - PRETTY GOOD
 (1), (2) > (3)				|	(3)	8  :  34 :   68		-> (80/2) + (340/4) = 40 + 85 = 125 - START GOING DOWN AT ABOUT 100
 
 TIME: 240 (1) - Speed: 149, Jump: 933, Gravity: 3196, Walk: 400 ->	3196/933 = 3.4!
 equates to: (149/3.4) + (933/2)/3.4 = 181! [hence jumps 2 platforms!] + [Majority comes from (j:g) not speed, hence we go up]... MAKING SENSE
 
 1 and 2 work well!
 ----------------
 
 */

#include "GameDirector.h"
#include "Global.h"

int main( int argc, char* args[] )
{	
	srand(time(NULL));
	
    //Global initialises all SDL subsytems
    Global::sharedGlobal();
    
    //Game director controls the flow of the game
	GameDirector* director;
	director = GameDirector::sharedDirector();
	
	director->gameLoop();
	
	director->cleanUp();
	
	return 0;
}
