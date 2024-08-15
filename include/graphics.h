#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef enum
{
    FONT_BANGERS_SM,
    FONT_BANGERS_LG,
    FONT_BREESERIF_SM,
    FONT_BREESERIF_LG
} Font;

typedef enum
{
    COLOR_WHITE,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE
} Color;

typedef struct
{
    int x, y;
    int width, height;
    SDL_Texture *texture;
} GameObject;

int initGraphics();
void closeGraphics();
GameObject *createGameObject(const char *texture_path, int x, int y, int width, int height);
void destroyGameObject(GameObject *obj);
void moveGameObject(GameObject *obj, int dx, int dy);
void renderGameObject(GameObject *obj);

#endif // GRAPHICS_H
