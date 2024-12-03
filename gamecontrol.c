#ifndef CONTROL 
#define CONTROL 


#include "terrain.c"
#include "arrows.c"
#include "player.c"
#include "ghosts.c"
#include "pickups.c"
#include "sparkle.c"

//====================================================================================
// Controll
//====================================================================================
void goToNextLevel(){
    
    float a = stageCounter;
    resetPickups();
    resetArrows();
    resetGhosts();
    resetTerrain();
    playerRespawn();
    resetSparkles();

    stageCounter = a + 1;
    theme = stageCounter % 3;
}


void resetGame(){
    resetPickups();
    resetArrows();
    resetGhosts();
    resetTerrain();
    playerReset();
    resetSparkles();
}


#endif