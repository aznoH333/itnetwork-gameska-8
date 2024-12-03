#ifndef ARROWS 
#define ARROWS 

#include "directions.c"
#include <stdlib.h>
#include "gframework.c"

//====================================================================================
// Arrows
//====================================================================================
struct Arrow {
    float x;
    float y;
    unsigned char direction;
    unsigned char lifeTime;
};typedef struct Arrow Arrow;


#define ARROW_SPEED 2.5f
#define ARROW_COUNT 32
Arrow* arrows[ARROW_COUNT];
int lastArrowIndex = 0;


void prepArrows(){
    for (int i = 0; i < ARROW_COUNT; i++){
        arrows[i] = 0;
    }
}


void resetArrows(){
    for (int i = 0; i < ARROW_COUNT; i++){
        if (arrows[i] != 0){
            free(arrows[i]);
        }
        arrows[i] = 0;
    }
    lastArrowIndex = 0;
}


void addArrow(float x, float y, int direction){
    lastArrowIndex++;
    lastArrowIndex %= ARROW_COUNT;
    
    if (arrows[lastArrowIndex] != 0){
        free(arrows[lastArrowIndex]);
    }
    arrows[lastArrowIndex] = malloc(sizeof(Arrow));
    arrows[lastArrowIndex]->x = x;
    arrows[lastArrowIndex]->y = y;
    arrows[lastArrowIndex]->direction = direction;
    arrows[lastArrowIndex]->lifeTime = 255;
}

int arrowDirrectionLookup[] = {270, 90, 180, 0};
void updateArrows(){
    for (int i = 0; i < ARROW_COUNT; i++){
        if (arrows[i] == 0){
            continue;
        }

        Arrow* a = arrows[i];

        a->lifeTime--;
        if (a->lifeTime == 0){
            free(arrows[i]);
            arrows[i] = 0;
            continue;
        }   

        int rotation = arrowDirrectionLookup[a->direction];
        a->x += directionLookup[a->direction].x * ARROW_SPEED;
        a->y += directionLookup[a->direction].y * ARROW_SPEED;

        drawR(16, a->x, a->y, rotation);
    }
}

#endif