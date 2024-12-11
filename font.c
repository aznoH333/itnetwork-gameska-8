#ifndef FONT 
#define FONT 

#include "raylib.h"

Font font;

void loadFont(){
    font = LoadFont("./resources/custom_font_detailed.png");
}

void drawFont(const char* text, float x, float y, float scale){
    DrawTextEx(font, text, (Vector2){x, y}, scale, 1.0f, WHITE);
}

void unloadFont(){
    UnloadFont(font);
}


#endif