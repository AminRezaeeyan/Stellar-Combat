#ifndef GAME_H
#define GAME_H

#define SDL_MAIN_HANDLED
#define MENU_OPTION_COUNT 3

typedef enum
{
    MENU_PLAY,
    MENU_RECORDS,
    MENU_EXIT
} MenuOption;

int setUpGame(void);
void closeGame(void);
void renderMenu(MenuOption selectedOption);
MenuOption handleMenuEvents();
int runGame(void);
void runGameLoop();
int handleGameEvents();

#endif