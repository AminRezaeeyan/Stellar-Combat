#include "game.h"
#include "logger.h"
#include "graphics.h"
#include <stdlib.h>

int setUpGame()
{
    if (initGraphics() != 0 || initLogger("../error.log") != 0)
    {
        return -1;
    }

    return 0;
}

void closeGame()
{
    closeGraphics();
    closeLogger();
}

int runGame()
{
    if (setUpGame() != 0)
    {
        return -1;
    }
    atexit(closeGame);

    renderBackground();

    int running = 1;
    while (running)
    {
        MenuOption selectedOption = handleMenuEvents();

        switch (selectedOption)
        {
        case MENU_PLAY:
            runGameLoop();
            break;
        case MENU_RECORDS:
            // TODO
            break;
        case MENU_EXIT:
            running = 0;
            break;
        }
    }
    return 0;
}

void renderMenu(MenuOption selectedOption)
{
    const char *options[MENU_OPTION_COUNT] = {"Play", "Records", "Exit"};
    Font font = FONT_BREE_SERIF_LG;
    Color normalColor = COLOR_WHITE;
    Color selectedColor = COLOR_GOLD;

    for (int i = 0; i < MENU_OPTION_COUNT; ++i)
    {
        Color color = (i == selectedOption) ? selectedColor : normalColor;
        renderText(options[i], font, color, 50 + i * 60, 250 + i * 140);
    }

    presentScreen();
}

MenuOption handleMenuEvents()
{
    GameObject *logo = createGameObject("../resources/assets/images/logo.png", WINDOW_WIDTH - 450, 0, 450, 450);
    renderGameObject(logo);

    renderText("Developed by: Amin Rezaeeyan", FONT_BREE_SERIF_SM, COLOR_WHITE, 10, WINDOW_HEIGHT - 50);
    renderText("Use Arrow Keys or WASD to Navigate", FONT_BREE_SERIF_SM, COLOR_WHITE, WINDOW_WIDTH - 310, WINDOW_HEIGHT - 50);

    MenuOption selectedOption = MENU_PLAY;
    int menuRunning = 1;

    while (menuRunning)
    {
        renderMenu(selectedOption);
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return MENU_EXIT;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                case SDLK_w:
                    if (selectedOption > 0)
                        selectedOption--;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    if (selectedOption < MENU_OPTION_COUNT - 1)
                        selectedOption++;
                    break;
                case SDLK_RETURN:
                case SDLK_KP_ENTER:
                    menuRunning = 0;
                    break;
                default:
                    break;
                }
            }
        }
    }
    return selectedOption;
}

void runGameLoop()
{
    int gameRunning = 1;
    while (gameRunning)
    {
        gameRunning = handleGameEvents();
        // Update game state
        // Render game objects
    }
}

int handleGameEvents()
{
    return 0;
}
