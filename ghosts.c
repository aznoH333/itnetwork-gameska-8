#ifndef GHOSTS 
#define GHOSTS 

#include "player.c"

//====================================================================================
// Ghosts
//====================================================================================
struct Ghost{
    float x;
    float y;
    int health;
}; typedef struct Ghost Ghost;
#define MAX_GHOSTS 64
Ghost* ghosts[MAX_GHOSTS];
int ghostCounter;
float ghostCooldown;


void prepGhosts(){
    for (int i = 0; i < MAX_GHOSTS; i++){
        ghosts[i] = 0;
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


void updateGhosts(){
    Color ghostColor = {255, 255, 255, sin(fTimer * 0.25) * 30 + 150};
    int ghostSprite = (fTimer / 12) % 4;

    for (int i = 0; i < MAX_GHOSTS; i++){
        Ghost* g = ghosts[i];

        if (g == 0){
            continue;
        }

        // arrow collisions
        for (int i = 0; i < ARROW_COUNT; i++){
            if (arrows[i] == 0){
                continue;
            }

            Arrow* a = arrows[i];

            if (checkBoxCollisions(a->x, a->y, 16, 16, g->x, g->y, 16, 16)){
                arrows[i] = 0;
                free(a);
                g->health -= damage;
                screenShake(1.2f);
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
        if (g->health < 0 || distanceToPlayer > 300){
            if (distanceToPlayer > 300){
                float a = GetRandomValue(0, PI*200)/100.0f;
                addGhost((sinf(a) * 250) + playerX, (cosf(a) * 250) + playerY);
            }else {
                screenShake(5.0f);
            }
            free(g);
            ghosts[i] = 0;
            break; 
        };

        // draw
        drawC(20 + ghostSprite, g->x, g->y, ghostColor);
    }

    
    if (ghostCooldown <= 0){
        float a = GetRandomValue(0, PI*200)/100.0f;

        addGhost((sinf(a) * 250) + playerX, (cosf(a) * 250) + playerY);

        ghostCooldown = 180.0f / ((stageCounter * 0.3f) + 1.0f);
    }else {
        ghostCooldown--;
    }


}


#endif