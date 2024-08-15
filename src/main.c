#include "game.h"
#include "logger.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (!initGame())
    {
        return 1;
    }

    return 0;
}
