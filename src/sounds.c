#include "sounds.h"

// Array to hold sound effects
static Mix_Chunk *soundEffects[10];
static Mix_Music *backgroundMusic = NULL;

int initSounds()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        LOG_ERROR("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }

    if (Mix_Init(MIX_INIT_OGG) < 0)
    {
        LOG_ERROR("Failed to initialize SDL_mixer! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }

    // Load background music
    backgroundMusic = loadMusic(SOUND_BACKGROUND);

    // Load sound effects
    soundEffects[SOUND_TYPE_SHOOT_1] = loadSoundEffect(SOUND_SHOOT_1);
    soundEffects[SOUND_TYPE_SHOOT_2] = loadSoundEffect(SOUND_SHOOT_2);
    soundEffects[SOUND_TYPE_EXPLOSION_1] = loadSoundEffect(SOUND_EXPLOSION_1);
    soundEffects[SOUND_TYPE_EXPLOSION_2] = loadSoundEffect(SOUND_EXPLOSION_2);
    soundEffects[SOUND_TYPE_EXPLOSION_3] = loadSoundEffect(SOUND_EXPLOSION_3);
    soundEffects[SOUND_TYPE_EXPLOSION_4] = loadSoundEffect(SOUND_EXPLOSION_4);
    soundEffects[SOUND_TYPE_HIT] = loadSoundEffect(SOUND_HIT);
    soundEffects[SOUND_TYPE_BONUS] = loadSoundEffect(SOUND_BONUS);
    soundEffects[SOUND_TYPE_GAME_OVER] = loadSoundEffect(SOUND_GAME_OVER);

    return 0;
}

void closeSounds()
{
    // Free sound effects
    for (int i = 0; i < 10; i++)
    {
        Mix_FreeChunk(soundEffects[i]);
        soundEffects[i] = NULL;
    }

    // Free background music
    Mix_FreeMusic(backgroundMusic);
    backgroundMusic = NULL;

    Mix_Quit();
}

Mix_Chunk *loadSoundEffect(const char *file)
{
    Mix_Chunk *sound = Mix_LoadWAV(file);
    if (sound == NULL)
    {
        LOG_ERROR("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }
    return sound;
}

Mix_Music *loadMusic(const char *file)
{
    Mix_Music *music = Mix_LoadMUS(file);
    if (music == NULL)
    {
        LOG_ERROR("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
    }
    return music;
}

void playSoundEffect(SoundType sound)
{
    if (sound >= 0 && sound < 10)
    {
        Mix_PlayChannel(-1, soundEffects[sound], 0);
    }
}

void playMusic(SoundType sound)
{
    if (sound == SOUND_TYPE_BACKGROUND && backgroundMusic != NULL)
    {
        if (Mix_PlayMusic(backgroundMusic, -1) == -1)
        {
            LOG_ERROR("Failed to play music! SDL_mixer Error: %s\n", Mix_GetError());
        }
    }
}

void stopMusic()
{
    Mix_HaltMusic();
}

void pauseMusic()
{
    Mix_PauseMusic();
}

void resumeMusic()
{
    Mix_ResumeMusic();
}

void setSoundVolume(int volume)
{
    Mix_Volume(-1, volume);
}

void setMusicVolume(int volume)
{
    Mix_VolumeMusic(volume);
}
