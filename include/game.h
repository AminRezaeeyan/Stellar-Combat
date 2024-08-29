#ifndef GAME_H
#define GAME_H

#include "logger.h"
#include "utils.h"
#include "graphics.h"
#include "menu.h"
#include "records.h"

#define BULLET_COOLDOWN 200 // miliseconds
#define BACKGROUND_SPEED 2  // px per frame

int setUpGame(void);
void closeGame(void);
int runGame(void);
void runGameLoop();
int handleEvents();
void renderGame();
void updateGame();
void renderMovingBackground();
void moveBackground();
void renderHealth();
void renderScore();
void updateScoreByInterval();
void displayGuide();
void pause();

#endif