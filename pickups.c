#ifndef PICKUPS 
#define PICKUPS 


#include "gframework.c"
#include <stdlib.h>
#include "player.c"




//====================================================================================
// Pickups
//====================================================================================
#define PICKUP_APPLE 0
#define PICKUP_WAND 1
#define PICKUP_GOLD 2

struct Pickup{
    float x;
    float y;
    unsigned char type;
}; typedef struct Pickup Pickup;
#define MAX_PICKUPS 10
Pickup* pickups[MAX_PICKUPS];
int pickupCounter = 0;

void prepPickups(){
    for (int i = 0; i < MAX_PICKUPS; i++){
        pickups[i] = 0;
    }
}


void addPickup(float x, float y){
    if (pickups[pickupCounter] != 0){
        free(pickups[pickupCounter]);
    }

    Pickup* p = malloc(sizeof(Pickup));
    p->x = x;
    p->y = y;
    p->type = GetRandomValue(0, 2);

    pickups[pickupCounter] = p;
    pickupCounter++;
    pickupCounter%=MAX_PICKUPS;
}

void resetPickups(){
    for (int i = 0; i < MAX_PICKUPS; i++){
        if (pickups[i] != 0){
            free(pickups[i]);
            pickups[i] = 0;
        }
    }
}

void pickupUpdate(){
    for (int i = 0; i < MAX_PICKUPS; i++){

        if (pickups[i] == 0){
            continue;
        }

        Pickup* p = pickups[i];

        if (checkBoxCollisions(p->x, p->y, 16, 16, playerX, playerY, 16, 16)){
            switch (p->type) {
                case PICKUP_APPLE:
                    score += 20 * (stageCounter + 1);
                    break;
                case PICKUP_WAND:
                    score += 5 * (stageCounter + 1);
                    if (fireCooldown > 20){
                        fireCooldown -= 1;
                    }
                    break;
                case PICKUP_GOLD:
                    score += 5 * (stageCounter + 1);
                    damage += 5;
                    break;
            }
            
            free(p);
            pickups[i] = 0;
            pickupCount--;
            if (pickupCount == 0){
                goToNextLevel();
            }
        }
        draw(p->type + 17, p->x, p->y);
    }
}

#endif