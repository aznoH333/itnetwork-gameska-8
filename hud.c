#ifndef HUD
#define HUD 

#include "font.c"
#include "terrain.c"
#include "player.c"
#include <stdio.h>


float scoreTimer = 0;

void updateHud(){
    // score
    char scoreText[255];
    sprintf(scoreText, "Score : %05d", score);

    float scoreScalingFactor = 1 + (scoreTimer * 0.03f);
    
    drawFont(scoreText, playerX - 128, playerY - 72 ,12.0f * scoreScalingFactor);

    scoreTimer -= scoreTimer > 0;


    // pickups
    char pickupText[255];
    sprintf(pickupText, "Items : %d/%d", pickupCount, totalPickups);
    drawFont(pickupText, playerX + 64, playerY - 72 ,12.0f);
}

void scoreHit(int ammount){
    scoreTimer += ammount;
}

#endif