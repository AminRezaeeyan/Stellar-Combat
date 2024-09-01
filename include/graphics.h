#ifndef GRAPHICS_H
#define GRAPHICS_H

// This header file provides an interface between the SDL library and the application.
// It includes functions for initializing and closing the graphics system, managing textures,
// creating and manipulating game objects, rendering various elements on the screen, and handling events.
// The goal is to abstract away the SDL-specific details and provide a simpler API for the game.

#define SDL_MAIN_HANDLED

#include "logger.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Constants for frame rate and window dimensions
#define FPS 60
#define FRAME_DELAY 1000 / FPS

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

// File paths for various game assets
#define ICON "../resources/assets/images/logo.png"
#define BACKGROUND "../resources/assets/images/background.png"
#define SPACESHIP "../resources/assets/images/spaceship.png"
#define FULL_HEART "../resources/assets/images/full-heart.png"
#define HALF_HEART "../resources/assets/images/half-heart.png"
// Enemies
#define MINE "../resources/assets/images/mine.png"
#define FIGHTER "../resources/assets/images/fighter.png"
#define DRONE "../resources/assets/images/drone.png"
#define BOMBER "../resources/assets/images/bomber.png"
// Rockets
#define FIRE_ROCKET "../resources/assets/images/bullet.png"
#define LARGE_ROCKET "../resources/assets/images/rocket.png"
#define SMALL_ROCKET "../resources/assets/images/long-rocket.png"
// Explosions
#define EXPLOSION_1 "../resources/assets/images/explosion-1.png"
#define EXPLOSION_2 "../resources/assets/images/explosion-2.png"
#define EXPLOSION_3 "../resources/assets/images/explosion-3.png"
#define GET_EXPLOSION(num) ((num) == 1 ? EXPLOSION_1 : (num) == 2 ? EXPLOSION_2 \
                                                                  : EXPLOSION_3)

// Enumeration for different font types
typedef enum
{
    FONT_BANGERS_SM,
    FONT_BANGERS_MD,
    FONT_BANGERS_LG,
    FONT_BREE_SERIF_SM,
    FONT_BREE_SERIF_MD,
    FONT_BREE_SERIF_LG
} Font;

// Enumeration for different colors
typedef enum
{
    COLOR_WHITE,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_GOLD
} Color;

// Enumeration for different event types
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

// Structure representing a game object
typedef struct
{
    int x, y;             // Position of the game object
    int width, height;    // Dimensions of the game object
    SDL_Texture *texture; // Texture of the game object
} GameObject;

// Initialization and Cleanup
int initGraphics(void);
void closeGraphics(void);

// Texture Management
SDL_Texture *loadTexture(const char *file);
void renderTexture(SDL_Texture *texture, int x, int y, int width, int height);

// GameObject Management
GameObject *createGameObject(const char *texture_path, int x, int y, int width, int height);
void destroyGameObject(GameObject *obj);
void moveGameObject(GameObject *obj, int dx, int dy);
void renderGameObject(GameObject *obj);
void setGameObjectOpacity(GameObject *object, float opacity);

// Rendering
void renderText(const char *text, Font font, Color color, int x, int y);
void renderLine(int x1, int y1, int x2, int y2, Color color);
void clearScreen(Color backgroundColor);
void presentScreen(void);

// Utility Graphic Functions
SDL_Color getSDLColor(Color color);
TTF_Font *getSDLFont(Font font);
int loadBackground(const char *filepath);
void renderBackground(int x, int y);
EventType pollEvent(void);
void waitForKey(void);
Uint32 getTicks(void);
void delay(Uint32 ms);

#endif // GRAPHICS_H