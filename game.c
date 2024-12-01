#include "gframework.c"
#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>


//====================================================================================
// Directions
//====================================================================================
#define DIRECTION_UP 0
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT 2
#define DIRECTION_RIGHT 3
struct Tuple {char x; char y;}; typedef struct Tuple Tuple;
Tuple directionLookup[4] = {
    (Tuple){0,-1},
    (Tuple){0,1},
    (Tuple){-1,0},
    (Tuple){1,0},
};


//====================================================================================
// Terrain
//====================================================================================
unsigned char theme;
unsigned int stageCounter;
#define WORLD_SIZE 16
#define WORLD_TILE_SIZE 4
unsigned char world[WORLD_SIZE][WORLD_SIZE];
void resetTerrain(){
    theme = 0;
    stageCounter = 0;
    
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
}


void drawSquare(int x, int y){
    for (int i = 0; i < 4; i ++){
        for (int j = 0; j < 4; j++){
            draw(theme, ((x * WORLD_TILE_SIZE) + i) * 16, ((y * WORLD_TILE_SIZE) + j) * 16);
        }
    }
}


void updateTerrain(){
    // world
    for (int x = 0; x < WORLD_SIZE; x++){
        for (int y = 0; y < WORLD_SIZE; y++){
            if (world[x][y] == 0){
                continue;
            }
            drawSquare(x, y);
        }
    }

    // world border
    for (int i = -1; i < WORLD_SIZE + 1; i++){
        drawSquare(i, -1);
        drawSquare(i, WORLD_SIZE);
        drawSquare(-1, i);
        drawSquare(WORLD_SIZE, i);
    }
}


bool canWalk(float x, float y){
    return true;
}


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

int arrowDirrectionLookup[] = {270, 90, 0, 180};
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


//====================================================================================
// Player
//====================================================================================
float playerX;
float playerY;
bool playerFlip;
float playerWantsToMoveX;
float playerWantsToMoveY;
float cooldown;
float fireCooldown;
#define PLAYER_WALK_SPEED 1.7f;
int playerSprite;


void playerReset(){
    playerX = (WORLD_SIZE >> 1) * WORLD_TILE_SIZE * 16 + 24;
    playerY = (WORLD_SIZE >> 1) * WORLD_TILE_SIZE * 16 + 24;
    playerFlip = false;
    cooldown = 0;
    fireCooldown = 30.0f;
}


void playerUpdate(){
    bool isMoving = false;
    playerWantsToMoveX = 0;
    playerWantsToMoveY = 0;

    // movement inputs
    if (IsKeyDown(KEY_A)){
        playerWantsToMoveX = -PLAYER_WALK_SPEED;
        playerFlip = true;
    }else if (IsKeyDown(KEY_D)){
        playerWantsToMoveX = PLAYER_WALK_SPEED;
        playerFlip = false;
    }
    if (IsKeyDown(KEY_W)){
        playerWantsToMoveY = -PLAYER_WALK_SPEED;
    }else if (IsKeyDown(KEY_S)){
        playerWantsToMoveY = PLAYER_WALK_SPEED;
    }

    // update pos
    if (canWalk(playerX + playerWantsToMoveX, playerY) && playerWantsToMoveX != 0){
        isMoving = true;
        playerX += playerWantsToMoveX;
    }
    if (canWalk(playerX, playerWantsToMoveY + playerY) && playerWantsToMoveY != 0){
        isMoving = true;
        playerY += playerWantsToMoveY;
    }

    // shoot
    if (cooldown > 0){
        cooldown--;
    }else {
        if (IsKeyDown(KEY_LEFT)){
            addArrow(playerX, playerY, DIRECTION_LEFT);
            cooldown = fireCooldown;
        }else if (IsKeyDown(KEY_RIGHT)) {
            addArrow(playerX, playerY, DIRECTION_RIGHT);
            cooldown = fireCooldown;
        }else if (IsKeyDown(KEY_UP)){
            addArrow(playerX, playerY, DIRECTION_UP);
            cooldown = fireCooldown;
        }else if (IsKeyDown(KEY_DOWN)){
            addArrow(playerX, playerY, DIRECTION_DOWN);
            cooldown = fireCooldown;
        }
    }

    //draw
    int sprite = 0;
    if (isMoving){
        sprite = (fTimer >> 1) % 6;
    }
    sprite += playerFlip * 6;
    setCameraPos((Vector2){playerX + 8, playerY + 8});
    draw(sprite + 4, playerX, playerY);
}


//====================================================================================
// Main
//====================================================================================
int main(void)
{
    initFramework();
    prepArrows();
    playerReset();
    resetArrows();
    ToggleFullscreen();
    resetTerrain();

    // Main game loop
    while (!WindowShouldClose())
    {
        fDrawBegin();
            ClearBackground(BLACK);
            playerUpdate();
            updateTerrain();            
            updateArrows();
        fDrawEnd();
    }

	disposeFramework();
    

    return 0;
}
