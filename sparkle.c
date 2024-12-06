#ifndef SPARKLE
#define SPARKLE 

#include <stdlib.h>
#include "directions.c"
#include "gframework.c"

#define MAX_LIFETIME 30
#define MAX_SPARKLES 25
struct Sparkle {
    float x;
    float y;
    unsigned char direction;
    unsigned char lifeTime;
}; typedef struct Sparkle Sparkle;

Sparkle* sparkles[MAX_SPARKLES];

void prepSparkles(){
    for (int i = 0; i < MAX_SPARKLES; i++){
        sparkles[i] = 0;
    }
}


void resetSparkles(){
    for (int i = 0; i < MAX_SPARKLES; i++){
        if (sparkles[i] != 0){
            free(sparkles[i]);
        }

        sparkles[i] = 0;
    }
}


void updateSparkles(){
    for (int i = 0; i < MAX_SPARKLES; i++){
        if (sparkles[i] == 0){
            continue;
        }


        Sparkle* s = sparkles[i];

        // move
        s->x += directionLookup[s->direction].x * 0.3f;
        s->y += directionLookup[s->direction].y * 0.3f;

        // update lifetime
        s->lifeTime++;
        if (s->lifeTime >= MAX_LIFETIME){
            free(s);
            sparkles[i] = 0;
            continue;
        }

        // draw
        int sprite = floorf((s->lifeTime / (float)MAX_LIFETIME ) * 3);

        draw(sprite + 28, s->x, s->y);
    }
}


void addSparkle(int x, int y){
    for (int i = 0; i < MAX_SPARKLES; i++){
        if (sparkles[i] != 0){
            continue;
        }

        Sparkle* sparkle = malloc(sizeof(Sparkle));

        sparkle->x = x;
        sparkle->y = y;
        sparkle->lifeTime = 0;
        sparkle->direction = GetRandomValue(0, 3);

        sparkles[i] = sparkle;
        break;
    }
}



#endif