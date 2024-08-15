#include "graphics.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *fontBangers_SM = NULL;
static TTF_Font *fontBangers_LG = NULL;
static TTF_Font *fontBreeSerif_SM = NULL;
static TTF_Font *fontBreeSerif_LG = NULL;

int initGraphics()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        LOG_ERROR("SDL could not initialize: %s", SDL_GetError());
        closeGraphics();
        return 0;
    }

    // Create window
    window = SDL_CreateWindow("Stellar Combat",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);

    if (!window)
    {
        LOG_ERROR("Window could not be created: %s", SDL_GetError());
        closeGraphics();
        return 0;
    }

    // Create renderer for window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        LOG_ERROR("Renderer could not be created: %s", SDL_GetError());
        closeGraphics();
        return 0;
    }

    // Initialize renderer color
    // SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        LOG_ERROR("SDL_image could not initialize: %s", IMG_GetError());
        closeGraphics();
        return 0;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        LOG_ERROR("SDL_ttf could not initialize: %s", TTF_GetError());
        closeGraphics();
        return 0;
    }

    // Load fonts
    fontBangers_SM = TTF_OpenFont("assets/fonts/Bangers.ttf", 16);
    fontBangers_LG = TTF_OpenFont("assets/fonts/Bangers.ttf", 32);
    fontBreeSerif_SM = TTF_OpenFont("assets/fonts/BreeSerif.ttf", 16);
    fontBreeSerif_LG = TTF_OpenFont("assets/fonts/BreeSerif.ttf", 32);
    if (!fontBangers_SM || !fontBangers_LG || !fontBreeSerif_SM || !fontBreeSerif_LG)
    {
        LOG_ERROR("Failed to load fonts: %s", TTF_GetError());
        closeGraphics();
        return 0;
    }

    return 1;
}

void closeGraphics()
{
    if (fontBangers_SM)
    {
        TTF_CloseFont(fontBangers_SM);
        fontBangers_SM = NULL;
    }
    if (fontBangers_LG)
    {
        TTF_CloseFont(fontBangers_LG);
        fontBangers_LG = NULL;
    }
    if (fontBreeSerif_SM)
    {
        TTF_CloseFont(fontBreeSerif_SM);
        fontBreeSerif_SM = NULL;
    }
    if (fontBreeSerif_LG)
    {
        TTF_CloseFont(fontBreeSerif_LG);
        fontBreeSerif_LG = NULL;
    }

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    // Destroy window
    if (window)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    // Quit SDL_ttf
    TTF_Quit();

    // Quit SDL_image
    IMG_Quit();

    // Quit SDL
    SDL_Quit();
}

GameObject *createGameObject(const char *texture_path, int x, int y, int width, int height)
{
    GameObject *obj = (GameObject *)malloc(sizeof(GameObject));
    if (!obj)
    {
        LOG_ERROR("Failed to allocate memory for GameObject");
        return NULL;
    }

    obj->x = x;
    obj->y = y;
    obj->width = width;
    obj->height = height;
    obj->texture = loadTexture(texture_path);

    if (!obj->texture)
    {
        free(obj);
        return NULL;
    }

    return obj;
}

void destroyGameObject(GameObject *obj)
{
    if (obj)
    {
        removeTexture(obj->texture);
        free(obj);
    }
}

void moveGameObject(GameObject *obj, int dx, int dy)
{
    if (obj)
    {
        obj->x += dx;
        obj->y += dy;
    }
}

void renderGameObject(GameObject *obj)
{
    if (obj && obj->texture)
    {
        renderTexture(obj->texture, obj->x, obj->y);
    }
}

void renderTexture(SDL_Texture *texture, int x, int y)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

SDL_Texture *loadTexture(const char *file)
{
    SDL_Texture *new_texture = NULL;
    SDL_Surface *loaded_surface = IMG_Load(file);
    if (loaded_surface == NULL)
    {
        LOG_ERROR("Unable to load image %s! SDL_image Error: %s\n", file, IMG_GetError());
        return NULL;
    }
    new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    SDL_FreeSurface(loaded_surface);
    return new_texture;
}

SDL_Color getSDLColor(Color color)
{
    switch (color)
    {
    case COLOR_WHITE:
        return (SDL_Color){255, 255, 255, 255};
    case COLOR_RED:
        return (SDL_Color){255, 0, 0, 255};
    case COLOR_GREEN:
        return (SDL_Color){0, 255, 0, 255};
    case COLOR_BLUE:
        return (SDL_Color){0, 0, 255, 255};
    default:
        return (SDL_Color){255, 255, 255, 255};
    }
}

TTF_Font *getSDLFont(Font font)
{
    switch (font)
    {
    case FONT_BANGERS_SM:
        return fontBangers_SM;
    case FONT_BANGERS_LG:
        return fontBangers_LG;
    case FONT_BREESERIF_SM:
        return fontBreeSerif_SM;
    case FONT_BREESERIF_LG:
        return fontBreeSerif_LG;
    default:
        return fontBangers_SM;
    }
}
