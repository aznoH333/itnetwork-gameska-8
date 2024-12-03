#include "gframework.c"
#include "raylib.h"
#include <stdlib.h>


#include "terrain.c"
#include "arrows.c"
#include "player.c"
#include "ghosts.c"
#include "pickups.c"
#include "sparkle.c"
#include "gamecontrol.c"


//====================================================================================
// Main
//====================================================================================
int main(void)
{
    initFramework();

    prepPickups();
    resetPickups();
    prepArrows();
    playerReset();
    resetArrows();
    resetTerrain();
    prepGhosts();
    resetGhosts();
    prepSparkles();
    resetSparkles();


    // Main game loop
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_P)){
            ToggleBorderlessWindowed();
        }
        
        fDrawBegin();
            ClearBackground(BLACK);
            updateTerrain();
            playerUpdate();
            pickupUpdate();            
            updateArrows();
            updateGhosts();
            updateSparkles();
        fDrawEnd();
    }

	disposeFramework();
    

    return 0;
}
