#ifndef GRAPHICS_H
#define GRAPHICS_H

#define SDL_MAIN_HANDLED

#include "logger.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define FPS 60
#define FRAME_DELAY 1000 / FPS

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define BACKGROUND "../resources/assets/images/background.png"
#define SPACESHIP "../resources/assets/images/spaceship.png"
#define FULL_HEART "../resources/assets/images/full-heart.png"
#define HALF_HEART "../resources/assets/images/half-heart.png"
#define BULLET "../resources/assets/images/bullet.png"

typedef enum
{
    FONT_BANGERS_SM,
    FONT_BANGERS_MD,
    FONT_BANGERS_LG,
    FONT_BREE_SERIF_SM,
    FONT_BREE_SERIF_MD,
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

typedef enum
{
    EVENT_NONE,
    EVENT_QUIT,
    EVENT_KEY_ESC,
    EVENT_KEY_UP,
    EVENT_KEY_DOWN,
    EVENT_KEY_LEFT,
    EVENT_KEY_RIGHT,
    EVENT_KEY_W,
    EVENT_KEY_A,
    EVENT_KEY_S,
    EVENT_KEY_D,
    EVENT_KEY_P,
    EVENT_KEY_ENTER,
    EVENT_KEY_SPACE,
    EVENT_KEY_OTHER
} EventType;

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
void renderLine(int x1, int y1, int x2, int y2, Color color);
void clearScreen(Color backgroundColor);
void presentScreen();
SDL_Color getSDLColor(Color color);
TTF_Font *getSDLFont(Font font);
void renderTexture(SDL_Texture *texture, int x, int y, int width, int height);
int loadBackground(const char *filepath);
void renderBackground(int x, int y);
EventType pollEvent();
void waitForKey();
Uint32 getTicks();
void delay(Uint32 ms);

#endif // GRAPHICS_H