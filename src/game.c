#include "game.h"

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
        MenuOption selectedOption = runMenu();

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
