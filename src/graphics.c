#include "graphics.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *fontBangers_SM = NULL;
static TTF_Font *fontBangers_MD = NULL;
static TTF_Font *fontBangers_LG = NULL;
static TTF_Font *fontBreeSerif_SM = NULL;
static TTF_Font *fontBreeSerif_MD = NULL;
static TTF_Font *fontBreeSerif_LG = NULL;
static SDL_Texture *backgroundTexture = NULL;

int initGraphics()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        LOG_ERROR("SDL could not initialize: %s", SDL_GetError());
        closeGraphics();
        return -1;
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
        return -1;
    }

    // Create renderer for window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        LOG_ERROR("Renderer could not be created: %s", SDL_GetError());
        closeGraphics();
        return -1;
    }

    // Initialize renderer color
    // SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        LOG_ERROR("SDL_image could not initialize: %s", IMG_GetError());
        closeGraphics();
        return -1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        LOG_ERROR("SDL_ttf could not initialize: %s", TTF_GetError());
        closeGraphics();
        return -1;
    }

    // Load fonts
    fontBangers_SM = TTF_OpenFont("../resources/assets/fonts/Bangers.ttf", 36);
    fontBangers_MD = TTF_OpenFont("../resources/assets/fonts/Bangers.ttf", 42);
    fontBangers_LG = TTF_OpenFont("../resources/assets/fonts/Bangers.ttf", 90);
    fontBreeSerif_SM = TTF_OpenFont("../resources/assets/fonts/BreeSerif.ttf", 18);
    fontBreeSerif_MD = TTF_OpenFont("../resources/assets/fonts/BreeSerif.ttf", 40);
    fontBreeSerif_LG = TTF_OpenFont("../resources/assets/fonts/BreeSerif.ttf", 84);
    if (!fontBangers_SM || !fontBangers_MD || !fontBangers_LG || !fontBreeSerif_SM || !fontBreeSerif_MD || !fontBreeSerif_LG)
    {
        LOG_ERROR("Failed to load fonts: %s", TTF_GetError());
        closeGraphics();
        return -1;
    }

    // set window icon
    SDL_Surface *iconSurface = IMG_Load("../resources/assets/images/logo.png");
    if (!iconSurface)
    {
        LOG_ERROR("Window icon could not be loaded: %s", IMG_GetError());
        closeGraphics();
        return -1;
    }
    SDL_SetWindowIcon(window, iconSurface);
    SDL_FreeSurface(iconSurface);

    // Load background
    if (loadBackground(BACKGROUND) != 0)
    {
        LOG_ERROR("Failed to load background");
        closeGraphics();
        return -1;
    }

    return 0;
}

void closeGraphics()
{
    if (fontBangers_SM)
    {
        TTF_CloseFont(fontBangers_SM);
        fontBangers_SM = NULL;
    }
    if (fontBangers_MD)
    {
        TTF_CloseFont(fontBangers_MD);
        fontBangers_MD = NULL;
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
    if (fontBreeSerif_MD)
    {
        TTF_CloseFont(fontBreeSerif_MD);
        fontBreeSerif_MD = NULL;
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
        SDL_DestroyTexture(obj->texture);
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
        renderTexture(obj->texture, obj->x, obj->y, obj->width, obj->height);
    }
}

void renderTexture(SDL_Texture *texture, int x, int y, int width, int height)
{
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    if (width > 0 && height > 0)
    {
        destRect.w = width;
        destRect.h = height;
    }
    else
    {
        SDL_QueryTexture(texture, NULL, NULL, &destRect.w, &destRect.h);
    }

    SDL_RenderCopy(renderer, texture, NULL, &destRect);
}

void renderText(const char *text, Font font, Color color, int x, int y)
{
    TTF_Font *ttfFont = getSDLFont(font);
    SDL_Color sdlColor = getSDLColor(color);
    SDL_Surface *surface = TTF_RenderText_Solid(ttfFont, text, sdlColor);

    if (!surface)
    {
        LOG_ERROR("Failed to create text surface! TTF_Error: %s\n", TTF_GetError());
        TTF_CloseFont(ttfFont);
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        LOG_ERROR("Failed to create text texture! SDL_Error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(ttfFont);
        return;
    }

    renderTexture(texture, x, y, -1, -1);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void clearScreen(Color color)
{
    SDL_Color sdlColor = getSDLColor(color);
    SDL_SetRenderDrawColor(renderer, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
    SDL_RenderClear(renderer);
}

void presentScreen()
{
    SDL_RenderPresent(renderer);
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
        return (SDL_Color){5, 226, 255, 255};
    case COLOR_GOLD:
        return (SDL_Color){204, 172, 0, 255};
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
    case FONT_BANGERS_MD:
        return fontBangers_MD;
    case FONT_BANGERS_LG:
        return fontBangers_LG;
    case FONT_BREE_SERIF_SM:
        return fontBreeSerif_SM;
    case FONT_BREE_SERIF_MD:
        return fontBreeSerif_MD;
    case FONT_BREE_SERIF_LG:
        return fontBreeSerif_LG;
    default:
        return fontBangers_SM;
    }
}

int loadBackground(const char *filePath)
{
    // Load the background texture
    backgroundTexture = loadTexture(filePath);
    if (!backgroundTexture)
    {
        LOG_ERROR("Failed to load background texture: %s", SDL_GetError());
        return -1;
    }
    return 0;
}

void renderBackground(int x, int y)
{
    if (backgroundTexture)
    {
        renderTexture(backgroundTexture, x, y, WINDOW_WIDTH, WINDOW_HEIGHT);
    }
}

EventType pollEvent()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return EVENT_QUIT;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                return EVENT_KEY_UP;
            case SDLK_DOWN:
                return EVENT_KEY_DOWN;
            case SDLK_LEFT:
                return EVENT_KEY_LEFT;
            case SDLK_RIGHT:
                return EVENT_KEY_RIGHT;
            case SDLK_w:
                return EVENT_KEY_W;
            case SDLK_a:
                return EVENT_KEY_A;
            case SDLK_s:
                return EVENT_KEY_S;
            case SDLK_d:
                return EVENT_KEY_D;
            case SDLK_p:
                return EVENT_KEY_P;
            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                return EVENT_KEY_ENTER;
            case SDLK_ESCAPE:
                return EVENT_KEY_ESC;
            case SDLK_SPACE:
                return EVENT_KEY_SPACE;
            default:
                return EVENT_KEY_OTHER;
            }
        }
    }
    return EVENT_NONE;
}

void waitForKey()
{
    EventType eventType = EVENT_NONE;
    while (eventType == EVENT_NONE)
    {
        eventType = pollEvent();
    }
    if (eventType == EVENT_QUIT)
        exit(0);
}

void renderLine(int x1, int y1, int x2, int y2, Color color)
{
    SDL_Color sdlColor = getSDLColor(color);
    SDL_SetRenderDrawColor(renderer, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

Uint32 getTicks()
{
    return SDL_GetTicks();
}

void delay(Uint32 ms)
{
    SDL_Delay(ms);
}