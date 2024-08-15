#include "game.h"
#include "logger.h"
#include "graphics.h"
#include <stdlib.h>

int initGame()
{
    atexit(closeGame);

    if (!initGraphics() || !initLogger("../error.log"))
    {
        return 0;
    }

    return 1;
}

void closeGame()
{
    closeGraphics();
    closeLogger();
}
