#ifndef SOUNDS
#define SOUNDS


#include "raylib.h"



#define SOUND_APPLE_PICKUP 0
#define SOUND_WAND_PICKUP 1
#define SOUND_GOLD_PICKUP 2
#define SOUND_GAME_OVER 3
#define SOUND_SHOOT 4
#define SOUND_PICKUP 5
#define SOUND_GHOST1 6
#define SOUND_GHOST2 7
#define SOUND_GHOST_DEATH 8
#define SOUND_EXIT_OPEN 9
#define MAX_SOUNDS SOUND_EXIT_OPEN + 1

Sound sounds[MAX_SOUNDS];

void loadSounds(){
    InitAudioDevice();
    
    sounds[SOUND_APPLE_PICKUP] = LoadSound("./resources/apple_pickup.wav");
    sounds[SOUND_WAND_PICKUP] = LoadSound("./resources/magic.wav");
    sounds[SOUND_GOLD_PICKUP] = LoadSound("./resources/gold_pickup.wav");
    sounds[SOUND_GAME_OVER] = LoadSound("./resources/game_over.wav");
    sounds[SOUND_SHOOT] = LoadSound("./resources/player_shoot.wav");
    sounds[SOUND_PICKUP] = LoadSound("./resources/pickup.wav");
    sounds[SOUND_GHOST1] = LoadSound("./resources/ghost_ambient1.wav");
    sounds[SOUND_GHOST2] = LoadSound("./resources/ghost_ambient2.wav");
    sounds[SOUND_GHOST_DEATH] = LoadSound("./resources/ghost_death.wav");
    sounds[SOUND_EXIT_OPEN] = LoadSound("./resources/the_exit_is_open.wav");
}

void unloadSounds(){
    CloseAudioDevice();
    for (int i = 0; i < MAX_SOUNDS; i++){
        UnloadSound(sounds[i]);
    }
}

void playSound(int sound, float volume){
    SetSoundVolume(sounds[sound], volume);
    PlaySound(sounds[sound]);
}



#endif