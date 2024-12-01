#include "gframework.c"
#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>



//====================================================================================
// Utils
//====================================================================================
float distanceTo(float x1, float y1, float x2, float y2){
    return sqrt(pow(x1 - x2,2) + pow(y1 - y2, 2));
}


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
#define WORLD_SIZE 16
#define WORLD_TILE_SIZE 4
unsigned char world[WORLD_SIZE][WORLD_SIZE];
unsigned int stageCounter;
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
    bool flipMove = false;
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

    float canWalkX = playerX + playerWantsToMoveX + (flipMove * 16);
    float canWalkY = playerY + playerWantsToMoveY + (flipMove * 16);

    // update pos
    if (canWalk(canWalkX, playerY) && playerWantsToMoveX != 0){
        isMoving = true;
        playerX += playerWantsToMoveX;
    }
    if (canWalk(playerX, canWalkY) && playerWantsToMoveY != 0){
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
        g->health = 170 + (stageCounter * 30);
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

        // move
        float dir = dirTowards(playerX, playerY, g->x, g->y);
        g->x += sinf(dir) * (0.5 + (stageCounter * 0.1));
        g->y += cosf(dir) * (0.5 + (stageCounter * 0.1));

        // destroy
        float distanceToPlayer = distanceTo(g->x, g->y, playerX, playerY);
        if (g->health < 0 || distanceToPlayer > 300){
            if (distanceToPlayer > 300){
                float a = GetRandomValue(0, PI*200)/100.0f;
                addGhost((sinf(a) * 250) + playerX, (cosf(a) * 250) + playerY);
            }
            free(g);
            ghosts[i] = 0;
            break; 
        };

        // draw
        drawC(20 + ghostSprite, g->x, g->y, ghostColor);
    }

    
    if (ghostCooldown == 0){
        float a = GetRandomValue(0, PI*200)/100.0f;

        addGhost((sinf(a) * 250) + playerX, (cosf(a) * 250) + playerY);

        ghostCooldown = 180.0f / ((stageCounter * 0.3f) + 1.0f);
    }else {
        ghostCooldown--;
    }


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
    prepGhosts();
    resetGhosts();

    // Main game loop
    while (!WindowShouldClose())
    {
        fDrawBegin();
            ClearBackground(BLACK);
            playerUpdate();
            updateTerrain();            
            updateArrows();
            updateGhosts();
        fDrawEnd();
    }

	disposeFramework();
    

    return 0;
}
