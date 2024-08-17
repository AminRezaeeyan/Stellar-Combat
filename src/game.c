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
    Font font = FONT_BANGERS_LG;
    Color normalColor = COLOR_WHITE;
    Color selectedColor = COLOR_RED;

    clearScreen(COLOR_BLUE);

    for (int i = 0; i < MENU_OPTION_COUNT; ++i)
    {
        Color color = (i == selectedOption) ? selectedColor : normalColor;
        renderText(options[i], font, color, 100 + i * 50, 200 + i * 100);
    }

    presentScreen();
}

MenuOption handleMenuEvents()
{
    MenuOption selectedOption = MENU_RECORDS;
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
                    selectedOption = (selectedOption - 1 + MENU_OPTION_COUNT) % MENU_OPTION_COUNT;
                    break;
                case SDLK_DOWN:
                    selectedOption = (selectedOption + 1) % MENU_OPTION_COUNT;
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
