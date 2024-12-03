#ifndef DIRECTIONS 
#define DIRECTIONS 

#include "utils.c"

//====================================================================================
// Directions
//====================================================================================
#define DIRECTION_UP 0
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT 2
#define DIRECTION_RIGHT 3
Tuple directionLookup[4] = {
    (Tuple){0,-1},
    (Tuple){0,1},
    (Tuple){-1,0},
    (Tuple){1,0},
};

#endif