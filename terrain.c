#ifndef TERRAIN 
#define TERRAIN

#include <stdbool.h>
#include "utils.c"
#include "raylib.h"
#include "directions.c"
#include "gframework.c"

//====================================================================================
// Terrain
//====================================================================================
void goToNextLevel();
void resetGame();
void addPickup(float x, float y);

unsigned char theme;
#define WORLD_SIZE 16
#define WORLD_TILE_SIZE 4
unsigned char world[WORLD_SIZE][WORLD_SIZE];
unsigned int stageCounter;
unsigned int pickupCount;
void resetTerrain(){
    theme = 0;
    stageCounter = 0;
    pickupCount = 0;
    
    // generation vals
    Tuple points[WORLD_SIZE * WORLD_SIZE];
    int pointCounter = 0;

    // reset world
    for (int x = 0; x < WORLD_SIZE; x++){
        for (int y = 0; y < WORLD_SIZE; y++){
            world[x][y] = 1;
        }
    }

    // generate init point
    points[pointCounter] = (Tuple){WORLD_SIZE>>1, WORLD_SIZE>>1};
    world[points[pointCounter].x][points[pointCounter].y] = 0;
    pointCounter++;
    int lastX;
    int lastY;

    // generate noodles
    while (pointCounter < 60){
        Tuple chosenPoint = points[GetRandomValue(0, ((pointCounter - 1)>>1)<<1)];

        int randomLength = GetRandomValue(2, 4);
        int x = chosenPoint.x;
        int y = chosenPoint.y;

        char dir = GetRandomValue(0, 3);

        for (int i = 0; i < randomLength; i++){
            if (x < 0 || x >= WORLD_SIZE || y < 0 || y >= WORLD_SIZE){
                break;
            }
            
            if (world[x][y] == 1){
                world[x][y] = 0;
                points[pointCounter++] = (Tuple){x, y};
            }

            // update pos
            x += directionLookup[dir].x;
            y += directionLookup[dir].y;
        }
    }

    
    for (int i = 0; i < GetRandomValue(5, 10); i++){
        Tuple chosenPoint = points[GetRandomValue(0, ((pointCounter - 1)>>1)<<1)];
        pickupCount++;
        addPickup(((chosenPoint.x * 4) + GetRandomValue(0, 3)) * 16, ((chosenPoint.y * 4) + GetRandomValue(0, 3)) * 16);
    }
}


void drawSquare(int x, int y, bool isWall){
    for (int i = 0; i < 4; i ++){
        for (int j = 0; j < 4; j++){
            int sprite = (theme * isWall) + (!isWall * 3);
            draw(sprite, ((x * WORLD_TILE_SIZE) + i) * 16, ((y * WORLD_TILE_SIZE) + j) * 16);
        }
    }
}


void updateTerrain(){
    // world
    for (int x = 0; x < WORLD_SIZE; x++){
        for (int y = 0; y < WORLD_SIZE; y++){
            drawSquare(x, y, world[x][y] == 1);
        }
    }

    // world border
    for (int i = -1; i < WORLD_SIZE + 1; i++){
        drawSquare(i, -1, true);
        drawSquare(i, WORLD_SIZE, true);
        drawSquare(-1, i, true);
        drawSquare(WORLD_SIZE, i, true);
    }
}


bool canWalk(float x, float y){
    int fixedX = ((int)roundf(x)) >> 4;
    int fixedY = ((int)roundf(y)) >> 4;

    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            int x = (fixedX + i) >> 2;
            int y = (fixedY + j) >> 2;
            
            if (x < 0 || x >= WORLD_SIZE || y < 0 || y >= WORLD_SIZE){
                return false;
            }
            
            if (world[x][y] == 1){
                return false;
            }
        }
    }
    return true;
}

#endif