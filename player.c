#ifndef PLAYER 
#define PLAYER 


#include "gframework.c"
#include "terrain.c"
#include "arrows.c"
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
float damage;
bool isDead;
unsigned int score;
#define PLAYER_WALK_SPEED 1.7f;
int playerSprite;


void playerRespawn(){
    playerX = (WORLD_SIZE >> 1) * WORLD_TILE_SIZE * 16 + 24;
    playerY = (WORLD_SIZE >> 1) * WORLD_TILE_SIZE * 16 + 24;
}


void playerReset(){
    playerX = (WORLD_SIZE >> 1) * WORLD_TILE_SIZE * 16 + 24;
    playerY = (WORLD_SIZE >> 1) * WORLD_TILE_SIZE * 16 + 24;
    playerFlip = false;
    cooldown = 0;
    fireCooldown = 45.0f;
    damage = 50;
    score = 0;
    isDead = false;

}


void playerUpdate(){
    if (isDead){
        if (IsKeyDown(KEY_R)){
            resetGame();
        }
        return;
    }
    
    
    
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

#endif