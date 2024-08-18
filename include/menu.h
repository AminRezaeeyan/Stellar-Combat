#include "graphics.h"

#define MENU_OPTION_COUNT 3

typedef enum
{
    MENU_PLAY,
    MENU_RECORDS,
    MENU_EXIT
} MenuOption;

void renderMenu(MenuOption selectedOption);
MenuOption runMenu();