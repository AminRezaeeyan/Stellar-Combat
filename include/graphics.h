#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

typedef enum
{
    FONT_BANGERS_SM,
    FONT_BANGERS_LG,
    FONT_BREE_SERIF_SM,
    FONT_BREE_SERIF_LG
} Font;

typedef enum
{
    COLOR_WHITE,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_GOLD
} Color;

typedef struct
{
    int x, y;
    int width, height;
    SDL_Texture *texture;
} GameObject;

int initGraphics();
void closeGraphics();
SDL_Texture *loadTexture(const char *file);
GameObject *createGameObject(const char *texture_path, int x, int y, int width, int height);
void destroyGameObject(GameObject *obj);
void moveGameObject(GameObject *obj, int dx, int dy);
void renderGameObject(GameObject *obj);
void renderText(const char *text, Font font, Color color, int x, int y);
void clearScreen(Color backgroundColor);
void presentScreen();
SDL_Color getSDLColor(Color color);
TTF_Font *getSDLFont(Font font);
void renderTexture(SDL_Texture *texture, int x, int y, int width, int height);
int loadBackground(const char *filepath);
void renderBackground(void);

#endif // GRAPHICS_H
