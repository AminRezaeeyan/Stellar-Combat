#ifndef SOUNDS_H
#define SOUNDS_H

#include "logger.h"
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

// File paths for various sound effects
#define SOUND_SHOOT_1 "../resources/assets/sounds/shoot-1.wav"
#define SOUND_SHOOT_2 "../resources/assets/sounds/shoot-2.wav"

#define SOUND_EXPLOSION_1 "../resources/assets/sounds/explosion-1.wav"
#define SOUND_EXPLOSION_2 "../resources/assets/sounds/explosion-2.wav"
#define SOUND_EXPLOSION_3 "../resources/assets/sounds/explosion-3.wav"
#define SOUND_EXPLOSION_4 "../resources/assets/sounds/explosion-4.wav"

#define SOUND_HIT "../resources/assets/sounds/small-explosion.wav"
#define SOUND_BONUS "../resources/assets/sounds/bonus.wav"
#define SOUND_GAME_OVER "../resources/assets/sounds/gameover.wav"
#define SOUND_BACKGROUND "../resources/assets/sounds/background.ogg"

// Enumeration for different sound types
typedef enum
{
    SOUND_TYPE_SHOOT_1,
    SOUND_TYPE_SHOOT_2,
    SOUND_TYPE_EXPLOSION_1,
    SOUND_TYPE_EXPLOSION_2,
    SOUND_TYPE_EXPLOSION_3,
    SOUND_TYPE_EXPLOSION_4,
    SOUND_TYPE_HIT,
    SOUND_TYPE_BONUS,
    SOUND_TYPE_GAME_OVER,
    SOUND_TYPE_BACKGROUND
} SoundType;

// Initialization and Cleanup
int initSounds(void);
void closeSounds(void);

// Sound Management
Mix_Chunk *loadSoundEffect(const char *file);
Mix_Music *loadMusic(const char *file);
void playSoundEffect(SoundType sound);
void playMusic(SoundType sound);
void stopMusic(void);
void pauseMusic(void);
void resumeMusic(void);
void setSoundVolume(int volume);
void setMusicVolume(int volume);

#endif // SOUND_H
