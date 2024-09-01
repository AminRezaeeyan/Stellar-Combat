#ifndef GAME_H
#define GAME_H

// This header file defines the core structures and functions for the game.
// It includes constants, enumerations, and structures for the player, enemies, and explosions.
// The functions declared here handle game setup, updates, rendering, event handling, and more.

#include "logger.h"
#include "utils.h"
#include "graphics.h"
#include "menu.h"
#include "records.h"
#include <stdlib.h>

// Constants
#define BACKGROUND_SPEED 2         // Speed of the background movement in pixels per frame
#define MAX_PLAYER_MISSILES 10     // Maximum number of player's missiles
#define MAX_ENEMY_MISSILES 100     // Maximum number of enemies' missiles
#define MAX_ENEMIES 8              // Maximum number of enemies
#define MIN_ENEMIES 3              /// Minimum number of enemies
#define MAX_EXPLOSIONS MAX_ENEMIES // Maximum number of explosions
#define EXPLOSION_DURATION 3000    // Duration of explosions (ms)

// Enumeration for enemy types
typedef enum
{
    ENEMY_MINE,
    ENEMY_FIGHTER,
    ENEMY_DRONE,
    ENEMY_BOMBER
} EnemyType;

// Enumeration for missile types
typedef enum
{
    FIRE_MISSILE,
    SMALL_MISSILE,
    LARGE_MISSILE
} MissileType;

// Structure for the player
typedef struct
{
    GameObject *object;
    float health;
    int score;
    Uint32 lastShootTime;
    Uint32 shootCooldown;
} Player;

// Structure for enemies
typedef struct
{
    EnemyType type;
    GameObject *object;
    int health;
    Uint32 lastShootTime;
    Uint32 shootCooldown;
} Enemy;

// Structure for missiles
typedef struct
{
    MissileType type;
    GameObject *object;
    int speed;
} Missile;

// Structure for enemies
typedef struct
{
    GameObject *object;
    Uint32 startTime;
    Uint32 duration;
    float opacity;
} Explosion;

// Game Setup and Cleanup
int setUpGame(void);
void closeGame(void);
void resetGame(void);

// Game loop
int runGame(void);
void runGameLoop(void);
void gameOver(void);
void pause(void);

// Event handling
int handleEvents(void);

// Rendering
void renderGame(void);
void renderMovingBackground(void);
void renderHealth(void);
void renderScore(void);
void renderGuide(void);
void renderEnemies(void);
void renderMissiles(void);
void renderExplosions(void);

// Game State Updates
void updateGame(void);
void updateScoreByInterval(void);
void updateMissiles(void);
void updateExplosions(void);
void updateEnemies(void);
void updateDifficulty(void);

// Game Mechanics
void spawnEnemies(void);
Enemy createEnemy(EnemyType type);
void createExplosion(GameObject *gameObject);
int checkCollision(GameObject *a, GameObject *b);
void handleCollisions(void);
int findAvailableSpawnSpace(int width);
int isPositionOccupied(int x, int width);

#endif
