#ifndef GHOSTS 
#define GHOSTS 

#include "player.c"
#include "sparkle.c"
#include <stdlib.h>
#include <stdio.h>

//====================================================================================
// Ghosts
//====================================================================================
struct Ghost{
    float x;
    float y;
    int health;
}; typedef struct Ghost Ghost;

struct DeadGhost{
    float x;
    float y;
    unsigned char lifeTime;
}; typedef struct DeadGhost DeadGhost;
#define DEAD_GHOST_LIFETIME 25
#define MAX_DEAD_GHOSTS 12

#define MAX_GHOSTS 64
Ghost* ghosts[MAX_GHOSTS];
DeadGhost* deadGhosts[MAX_DEAD_GHOSTS];
int ghostCounter;
int deadGhostCounter;
float ghostCooldown;
#define INVIS_DISTANCE 145.0f

void prepGhosts(){
    for (int i = 0; i < MAX_GHOSTS; i++){
        ghosts[i] = 0;
    }

    for (int i = 0; i < MAX_DEAD_GHOSTS; i++){
        deadGhosts[i] = 0;
    }
}


void resetGhosts(){
    for (int i = 0; i < MAX_GHOSTS; i++){
        if (ghosts[i] != 0){
            free(ghosts[i]);
        }
        ghosts[i] = 0;
    }
    ghostCounter = 0;

    for (int i = 0; i < MAX_DEAD_GHOSTS; i++){
        if (deadGhosts[i] != 0){
            free(deadGhosts[i]);
        }
        deadGhosts[i] = 0;
    }
    deadGhostCounter = 0;

    ghostCooldown = 60.0f;
}


void addGhost(float x, float y){
    for (int i = 0; i < MAX_GHOSTS; i++){
        ghostCounter++;
        ghostCounter%=MAX_GHOSTS;
        if (ghosts[ghostCounter] != 0){
            continue;
        }

        Ghost* g = malloc(sizeof(Ghost));
        g->x = x;
        g->y = y;
        g->health = 100 + (stageCounter * 30.0f);
        ghosts[ghostCounter] = g;
        break;
    }
}

void addDeadGhost(float x, float y){
    for (int i = 0; i < MAX_DEAD_GHOSTS; i++){
        
        
        deadGhostCounter++;
        deadGhostCounter %= MAX_DEAD_GHOSTS;

        if (deadGhosts[deadGhostCounter] != 0){
            continue;
        } 


        DeadGhost* g = malloc(sizeof(DeadGhost));
        g->x = x;
        g->y = y;
        g->lifeTime = 0;
        deadGhosts[deadGhostCounter] = g;
        break;
    }
}

void updateGhosts(){
    int ghostSprite = (fTimer / 12) % 4;

    for (int i = 0; i < MAX_GHOSTS; i++){
        Ghost* g = ghosts[i];

        if (g == 0){
            continue;
        }
        Color ghostColor = {255, 255, 255, sin(fTimer * 0.25) * 30 + 200};


        // arrow collisions
        for (int i = 0; i < ARROW_COUNT; i++){
            if (arrows[i] == 0){
                continue;
            }

            Arrow* a = arrows[i];

            if (checkBoxCollisions(a->x, a->y, 16, 16, g->x, g->y, 16, 16)){
                // spawn sparkles
                for (int j = 0; j < 3; j++){
                    addSparkle(GetRandomValue(a->x, a->x + 4), GetRandomValue(a->y, a->y + 4));
                }
                
                arrows[i] = 0;
                free(a);
                g->health -= damage;
                screenShake(1.0f);
            }
        }

        // player collisions
        if (checkBoxCollisions(g->x, g->y, 16, 16, playerX, playerY, 16, 16)){
            isDead = true;
        }

        // move
        float dir = dirTowards(playerX, playerY, g->x, g->y);
        g->x += sinf(dir) * (1.5f - (1.0f / ((stageCounter>>2) + 1)));
        g->y += cosf(dir) * (1.5f - (1.0f / ((stageCounter>>2) + 1)));

        // destroy
        float distanceToPlayer = distanceTo(g->x, g->y, playerX, playerY);
        if (g->health < 0 || distanceToPlayer > 300 || isDead){
            
            if (distanceToPlayer > 300){
                float a = GetRandomValue(0, PI*200)/100.0f;
                addGhost((sinf(a) * 250) + playerX, (cosf(a) * 250) + playerY);
            }else {
                if (!isDead){
                    screenShake(2.0f);
                }
                
                addDeadGhost(g->x, g->y);
            }
            free(g);
            ghosts[i] = 0;
            break; 
        };

        // draw
        if (distanceToPlayer >= INVIS_DISTANCE){
            continue;
        }

        float fadeFactor = (INVIS_DISTANCE - distanceToPlayer) / INVIS_DISTANCE;
        ghostColor.a = ghostColor.a * fadeFactor;

        drawC(20 + ghostSprite, g->x, g->y, ghostColor);
    }

    
    if (ghostCooldown <= 0){
        float a = GetRandomValue(0, PI*200)/100.0f;

        addGhost((sinf(a) * 250) + playerX, (cosf(a) * 250) + playerY);

        ghostCooldown = 180.0f / ((stageCounter * 0.3f) + 1.0f);
    }else {
        ghostCooldown--;
    }


    // dead ghosts
    for (int i = 0; i < MAX_DEAD_GHOSTS; i++){
        if (deadGhosts[i] == 0){
            continue;
        }
        DeadGhost* g = deadGhosts[i];

        // update life
        if (g->lifeTime < DEAD_GHOST_LIFETIME){
            g->lifeTime++;
        }else {
            free(g);
            deadGhosts[i] = 0;
            continue;
        }


        // draw
        float distanceToPlayer = distanceTo(g->x, g->y, playerX, playerY);

        if (distanceToPlayer >= INVIS_DISTANCE){
            continue;
        }
        Color ghostColor = {255, 255, 255, sin(fTimer * 0.25) * 30 + 200};
        float fadeFactor = (INVIS_DISTANCE - distanceToPlayer) / INVIS_DISTANCE;
        ghostColor.a = ghostColor.a * fadeFactor;
        
        int spriteIndex = floorf(((float)g->lifeTime / DEAD_GHOST_LIFETIME) * 4);
        
        drawC(24 + spriteIndex, g->x, g->y, ghostColor);
    }

}


#endif