#ifndef GAME_H
#define GAME_H

#include "logger.h"
#include "graphics.h"
#include "menu.h"

int setUpGame(void);
void closeGame(void);
int runGame(void);
void runGameLoop();
int handleGameEvents();

#endif