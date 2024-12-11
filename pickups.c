#ifndef PICKUPS 
#define PICKUPS 


#include "gframework.c"
#include <raylib.h>
#include <stdlib.h>
#include "player.c"
#include "sparkle.c"
#include "sounds.c"



//====================================================================================
// Pickups
//====================================================================================
#define PICKUP_APPLE 0
#define PICKUP_WAND 1
#define PICKUP_GOLD 2
#define PICKUP_EXIT 3
#define FADE_DISTANCE 60.0f

struct Pickup{
    float x;
    float y;
    unsigned char type;
}; typedef struct Pickup Pickup;
#define MAX_PICKUPS 10
Pickup* pickups[MAX_PICKUPS];
int pickupCounter = 0;
bool isExitOpen = false;
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

void addExit(float x, float y){
    if (pickups[pickupCounter] != 0){
        free(pickups[pickupCounter]);
    }

    Pickup* p = malloc(sizeof(Pickup));
    p->x = x;
    p->y = y;
    p->type = PICKUP_EXIT;

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
    isExitOpen = false;
}

void pickupUpdate(){
    for (int i = 0; i < MAX_PICKUPS; i++){

        if (pickups[i] == 0){
            continue;
        }

        Pickup* p = pickups[i];

        if (p->type == PICKUP_EXIT && isExitOpen == false){
            return;
        }
        // collisions
        if (checkBoxCollisions(p->x, p->y, 16, 16, playerX, playerY, 16, 16)){
            // spawn sparkles
            for (int j = 0; j < 5; j++){
                addSparkle(GetRandomValue(p->x, p->x + 4), GetRandomValue(p->y, p->y + 4));
            }
            

            // do effect
            switch (p->type) {
                case PICKUP_APPLE:
                    score += 20 * (stageCounter + 1);
                    playSound(SOUND_APPLE_PICKUP, 1.0f);
                    break;
                case PICKUP_WAND:
                    score += 5 * (stageCounter + 1);
                    if (fireCooldown > 20){
                        fireCooldown -= 1;
                    }
                    playSound(SOUND_WAND_PICKUP, 1.0f);

                    break;
                case PICKUP_GOLD:
                    score += 5 * (stageCounter + 1);
                    damage += 5;
                    playSound(SOUND_GOLD_PICKUP, 1.0f);

                    break;
                case PICKUP_EXIT:
                    goToNextLevel();
                    break;
            }
            
            free(p);
            pickups[i] = 0;
            pickupCount--;
            if (pickupCount == 0){
                isExitOpen = true;
            }
        }
        // sparkle
        if (fTimer % 10 == 0 && GetRandomValue(0, 3) == 1){
            addSparkle(p->x, p->y);
        }


        // draw
        Color c = {255, 255, 255, 255};
        float distanceToPlayer = distanceTo(p->x, p->y, playerX, playerY);

        if (distanceToPlayer >= FADE_DISTANCE){
            c.r = 0;
            c.g = 0;
            c.b = 0;
        }else {
            float fadeFactor = (FADE_DISTANCE - distanceToPlayer) / FADE_DISTANCE;
            c.r *= fadeFactor;
            c.g *= fadeFactor;
            c.b *= fadeFactor;
        }
        
        int sprite = p->type + 17;
        if (p->type == PICKUP_EXIT){
            sprite = 31 + ((fTimer >> 2) % 4);
        }

        drawC(sprite, p->x, p->y, c);
    }
}

#endif