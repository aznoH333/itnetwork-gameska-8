#ifndef UTILS 
#define UTILS 


#include <math.h>

//====================================================================================
// Utils
//====================================================================================
float distanceTo(float x1, float y1, float x2, float y2){
    return sqrt(pow(x1 - x2,2) + pow(y1 - y2, 2));
}

struct Tuple {char x; char y;}; typedef struct Tuple Tuple;


#endif