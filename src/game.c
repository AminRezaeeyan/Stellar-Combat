#include "game.h"

Player player;
Enemy enemies[MAX_ENEMIES];
Explosion explosions[MAX_EXPLOSIONS];
Missile playerMissiles[MAX_PLAYER_MISSILES];
Missile enemyMissiles[MAX_ENEMY_MISSILES];
GameObject *bonusHeart;

int currentMaxEnemies = MIN_ENEMIES;

int setUpGame()
{
    if (initGraphics() != 0 || initLogger("../error.log") != 0)
    {
        return -1;
    }
    atexit(closeGame);

    srand(time(NULL));

    return 0;
}

void closeGame()
{
    closeGraphics();
    closeLogger();
}

void resetGame()
{
    for (int i = 0; i < MAX_PLAYER_MISSILES; i++)
    {
        playerMissiles[i] = (Missile){.type = 0, .object = NULL, .speed = 0};
    }

    for (int i = 0; i < MAX_ENEMY_MISSILES; i++)
    {
        enemyMissiles[i] = (Missile){.type = 0, .object = NULL, .speed = 0};
    }

    for (int i = 0; i < currentMaxEnemies; i++)
    {
        enemies[i] = (Enemy){.type = 0, .object = NULL, .health = 0, .lastShootTime = 0, .shootCooldown = 0};
    }

    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        explosions[i] = (Explosion){.object = NULL, .duration = 0, .opacity = 0, .startTime = 0};
    }

    player.health = 5.0;
    player.score = 0;
    player.object = createGameObject(SPACESHIP, 340, 640, 120, 120);
    player.lastShootTime = getTicks();
    player.shootCooldown = 200;

    bonusHeart = NULL;

    currentMaxEnemies = MIN_ENEMIES;
}

int runGame()
{
    if (setUpGame() != 0)
    {
        return -1;
    }

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
            displayRecords();
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
    resetGame();
    renderGuide();

    Uint32 startTicks, endTicks, frameTime;

    int gameRunning = 1;
    while (gameRunning && player.health > 0)
    {
        // Start the frame
        startTicks = getTicks();

        // Process input
        gameRunning = handleEvents();

        // Update
        updateGame();

        // Render
        clearScreen(COLOR_WHITE);
        renderGame();
        presentScreen();

        // Finish the frame
        endTicks = getTicks();
        frameTime = endTicks - startTicks;
        if (FRAME_DELAY > frameTime)
        {
            delay(FRAME_DELAY - frameTime);
        }
    }
    gameOver();
}

void gameOver()
{
    delay(1500);
    Record record = createRecord(player.score);
    addRecord(record);

    int highScore = getHighestScore();
    char scoreString[10];
    char highScoreString[10];

    sprintf(scoreString, "%d", player.score);
    sprintf(highScoreString, "%d", highScore);

    clearScreen(COLOR_WHITE);
    renderBackground(0, 0);
    renderText("Game Over!", FONT_BANGERS_LG, COLOR_RED, 200, 200);
    renderText("Your Score:", FONT_BANGERS_MD, COLOR_BLUE, 250, 420);
    renderText(scoreString, FONT_BANGERS_MD, COLOR_WHITE, 450, 420);
    renderText("High Score:", FONT_BANGERS_MD, COLOR_BLUE, 250, 520);
    renderText(highScoreString, FONT_BANGERS_MD, COLOR_WHITE, 450, 520);
    renderText("Press any key to continue", FONT_BANGERS_SM, COLOR_GOLD, 200, 700);

    if (player.score == highScore)
        renderText("New Record!", FONT_BANGERS_MD, COLOR_GREEN, 300, 320);

    presentScreen();
    delay(1500);
    waitForKey();
}

void pause()
{
    renderText("Paused", FONT_BANGERS_MD, COLOR_RED, 350, 10);
    presentScreen();
    waitForKey();
}

int handleEvents()
{
    EventType event = pollEvent();
    switch (event)
    {
    case EVENT_KEY_ESC:
        return 0;
    case EVENT_QUIT:
        exit(0);
    case EVENT_KEY_UP:
    case EVENT_KEY_W:
        if (player.object->y > 0)
            moveGameObject(player.object, 0, -2);
        break;
    case EVENT_KEY_DOWN:
    case EVENT_KEY_S:
        if (player.object->y + player.object->height < WINDOW_HEIGHT)
            moveGameObject(player.object, 0, 2);
        break;
    case EVENT_KEY_LEFT:
    case EVENT_KEY_A:
        if (player.object->x > 0)
            moveGameObject(player.object, -6, 0);
        break;
    case EVENT_KEY_RIGHT:
    case EVENT_KEY_D:
        if (player.object->x + player.object->width < WINDOW_WIDTH)
            moveGameObject(player.object, 6, 0);
        break;
    case EVENT_KEY_SPACE:
        if (getTicks() - player.lastShootTime >= player.shootCooldown)
        {
            for (int i = 0; i < MAX_PLAYER_MISSILES; i++)
            {
                if (playerMissiles[i].object == NULL)
                {
                    Missile missile;
                    missile.type = FIRE_MISSILE;
                    missile.speed = 5;
                    missile.object = createGameObject(FIRE_ROCKET, player.object->x + player.object->width / 2 - 5, player.object->y - 25, 10, 50);
                    player.lastShootTime = getTicks();
                    playerMissiles[i] = missile;
                    break;
                }
            }
        }
        break;
    case EVENT_KEY_P:
        pause();
        break;
    default:
        break;
    }

    return 1;
}

void renderGame()
{
    renderMovingBackground();
    renderExplosions();
    renderBonusHeart();
    renderMissiles();
    renderEnemies();
    renderGameObject(player.object);
    renderHealth();
    renderScore();
}

void renderMovingBackground()
{
    static int backgroundY1 = 0;
    static int backgroundY2 = -WINDOW_HEIGHT;

    backgroundY1 += BACKGROUND_SPEED;
    backgroundY2 += BACKGROUND_SPEED;

    if (backgroundY1 >= WINDOW_HEIGHT)
    {
        backgroundY1 = -WINDOW_HEIGHT;
    }
    if (backgroundY2 >= WINDOW_HEIGHT)
    {
        backgroundY2 = -WINDOW_HEIGHT;
    }

    renderBackground(0, backgroundY1);
    renderBackground(0, backgroundY2);
}

void renderHealth()
{
    static GameObject *fullHeart = NULL;
    static GameObject *halfHeart = NULL;

    if (fullHeart == NULL)
    {
        fullHeart = createGameObject(FULL_HEART, 0, 0, 38, 38);
    }
    if (halfHeart == NULL)
    {
        halfHeart = createGameObject(HALF_HEART, 0, 0, 19, 38);
    }

    int fullHearts = (int)player.health;
    int halfHearts = (player.health - fullHearts >= 0.5) ? 1 : 0;

    int x = 10;
    int y = 10;

    // Render full hearts
    for (int i = 0; i < fullHearts; i++)
    {
        fullHeart->x = x;
        fullHeart->y = y;
        renderGameObject(fullHeart);
        x += 40;
    }

    // Render half heart
    if (halfHearts > 0)
    {
        halfHeart->x = x;
        halfHeart->y = y;
        renderGameObject(halfHeart);
        x += 40; // Move to the next position
    }
}

void renderScore()
{
    int digitsCount = countDigits(player.score);
    char scoreString[10];
    sprintf(scoreString, "%d", player.score);
    renderText(scoreString, FONT_BANGERS_MD, COLOR_WHITE, WINDOW_WIDTH - 20 * digitsCount - 5, 10);
}

void renderGuide()
{
    clearScreen(COLOR_WHITE);
    renderBackground(0, 0);
    renderText("Steer:", FONT_BANGERS_SM, COLOR_BLUE, 100, 100);
    renderText("Shoot:", FONT_BANGERS_SM, COLOR_BLUE, 100, 250);
    renderText("Pause:", FONT_BANGERS_SM, COLOR_BLUE, 100, 400);
    renderText("Exit:", FONT_BANGERS_SM, COLOR_BLUE, 100, 550);

    renderText("Use the arrow keys or WASD keys", FONT_BANGERS_SM, COLOR_WHITE, 200, 100);
    renderText("Press the space bar to fire your weapons", FONT_BANGERS_SM, COLOR_WHITE, 200, 250);
    renderText("Press P to pause the game", FONT_BANGERS_SM, COLOR_WHITE, 200, 400);
    renderText("Press ESC to exit the game", FONT_BANGERS_SM, COLOR_WHITE, 200, 550);

    renderText("Press any key to start the game!", FONT_BANGERS_SM, COLOR_GOLD, 100, 700);
    presentScreen();

    waitForKey();
}

void renderEnemies()
{
    for (int i = 0; i < currentMaxEnemies; i++)
    {
        if (enemies[i].object != NULL)
        {
            renderGameObject(enemies[i].object);
        }
    }
}

void renderMissiles()
{
    for (int i = 0; i < MAX_PLAYER_MISSILES; i++)
    {
        if (playerMissiles[i].object != NULL)
        {
            renderGameObject(playerMissiles[i].object);
        }
    }

    for (int i = 0; i < MAX_ENEMY_MISSILES; i++)
    {
        if (enemyMissiles[i].object != NULL)
        {
            renderGameObject(enemyMissiles[i].object);
        }
    }
}

void renderExplosions()
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (explosions[i].object != NULL)
        {
            renderGameObject(explosions[i].object);
        }
    }
}

void renderBonusHeart()
{
    if (bonusHeart != NULL)
    {
        renderGameObject(bonusHeart);
    }
}

void updateGame()
{
    updateScoreByInterval();
    updateDifficulty();
    updateEnemies();
    updateExplosions();
    updateMissiles();
    updateBonusHeart();

    spawnEnemies();
    handleCollisions();
}

void updateScoreByInterval()
{
    static Uint32 lastUpdateTime = 0;
    static const Uint32 updateInterval = 1000; // miliseconds

    Uint32 currentTime = getTicks();
    if (currentTime - lastUpdateTime >= updateInterval)
    {
        player.score += 5;
        lastUpdateTime = currentTime;
    }
}

void updateMissiles()
{
    for (int i = 0; i < MAX_PLAYER_MISSILES; i++)
    {
        if (playerMissiles[i].object != NULL)
        {
            moveGameObject(playerMissiles[i].object, 0, -1 * playerMissiles[i].speed);

            if (playerMissiles[i].object->y + playerMissiles[i].object->height <= 0)
            {
                playerMissiles[i].object = NULL;
            }
        }
    }

    for (int i = 0; i < MAX_ENEMY_MISSILES; i++)
    {
        if (enemyMissiles[i].object != NULL)
        {
            moveGameObject(enemyMissiles[i].object, 0, enemyMissiles[i].speed);

            if (enemyMissiles[i].object->y >= WINDOW_HEIGHT)
            {
                enemyMissiles[i].object = NULL;
            }
        }
    }
}

void updateExplosions()
{
    Uint32 currentTime = SDL_GetTicks();
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (explosions[i].object == NULL)
            continue;

        moveGameObject(explosions[i].object, 0, BACKGROUND_SPEED);

        Uint32 elapsedTime = currentTime - explosions[i].startTime;
        if (elapsedTime > explosions[i].duration)
        {
            explosions[i].object = NULL;
        }
        else
        {
            float remainingTime = explosions[i].duration - elapsedTime;
            explosions[i].opacity = remainingTime / explosions[i].duration;
            setGameObjectOpacity(explosions[i].object, explosions[i].opacity);
        }
    }
}

void updateEnemies()
{
    for (int i = 0; i < currentMaxEnemies; i++)
    {
        if (enemies[i].object != NULL)
        {
            switch (enemies[i].type)
            {
            case ENEMY_MINE:
                moveGameObject(enemies[i].object, 0, BACKGROUND_SPEED);
                break;
            case ENEMY_FIGHTER:
                moveGameObject(enemies[i].object, 0, BACKGROUND_SPEED + 1);
                // Shooting logic
                if (getTicks() - enemies[i].lastShootTime >= enemies[i].shootCooldown)
                {
                    for (int j = 0; j < MAX_ENEMY_MISSILES; j++)
                    {
                        if (enemyMissiles[j].object == NULL)
                        {
                            Missile missile;
                            missile.type = SMALL_MISSILE;
                            missile.speed = 6;
                            missile.object = createGameObject(SMALL_ROCKET, enemies[i].object->x + enemies[i].object->width / 2 - 5, enemies[i].object->y + 25, 10, 50);

                            enemies[i].lastShootTime = getTicks();
                            enemyMissiles[j] = missile;
                            break;
                        }
                    }
                }
                break;
            case ENEMY_DRONE:
                // Drones follow the player
                if (player.object->x + player.object->width / 2 < enemies[i].object->x + enemies[i].object->width / 2)
                {
                    moveGameObject(enemies[i].object, -1, BACKGROUND_SPEED + 1);
                }
                else if (player.object->x + player.object->width / 2 > enemies[i].object->x + enemies[i].object->width / 2)

                {
                    moveGameObject(enemies[i].object, 1, BACKGROUND_SPEED + 1);
                }
                else
                {
                    moveGameObject(enemies[i].object, 0, BACKGROUND_SPEED + 1);
                }
                break;
            case ENEMY_BOMBER:
                moveGameObject(enemies[i].object, 0, BACKGROUND_SPEED + 1);
                if (getTicks() - enemies[i].lastShootTime >= enemies[i].shootCooldown)
                {
                    for (int j = 0; j < MAX_ENEMY_MISSILES; j++)
                    {
                        if (enemyMissiles[j].object == NULL)
                        {
                            Missile missile;
                            missile.type = LARGE_MISSILE;
                            missile.speed = 4;
                            missile.object = createGameObject(LARGE_ROCKET, enemies[i].object->x + enemies[i].object->width / 2 - 5, enemies[i].object->y + 30, 10, 50);
                            enemies[i].lastShootTime = getTicks();
                            enemyMissiles[j] = missile;
                            break;
                        }
                    }
                }
                break;
            case ENEMY_HEALTH:
                moveGameObject(enemies[i].object, 0, BACKGROUND_SPEED + 1);
                break;
            }

            if (enemies[i].object->y > WINDOW_HEIGHT)
            {
                enemies[i].object = NULL;
            }
        }
    }
}

void updateBonusHeart()
{
    if (bonusHeart != NULL)
    {
        moveGameObject(bonusHeart, 0, BACKGROUND_SPEED);
        if (bonusHeart->height >= WINDOW_HEIGHT)
        {
            bonusHeart = NULL;
        }
    }
}

void updateDifficulty()
{
    currentMaxEnemies = MIN_ENEMIES + player.score / 200;
    if (currentMaxEnemies > MAX_ENEMIES)
        currentMaxEnemies = MAX_ENEMIES;
}

void spawnEnemies()
{
    int droneExists = 0;
    int bomberExists = 0;
    int bonusHeartSpaceshipExists = 0;

    // Check if there are already drones or bombers in the enemies array
    for (int i = 0; i < currentMaxEnemies; i++)
    {
        if (enemies[i].object == NULL)
            continue;

        if (enemies[i].type == ENEMY_DRONE)
        {
            droneExists = 1;
        }
        if (enemies[i].type == ENEMY_BOMBER)
        {
            bomberExists = 1;
        }
        if (enemies[i].type == ENEMY_HEALTH)
        {
            bonusHeartSpaceshipExists = 1;
        }
    }

    for (int i = 0; i < currentMaxEnemies; i++)
    {
        if (enemies[i].object == NULL)
        {
            EnemyType type;

            if (currentMaxEnemies > 5 && i > 4)
            {
                type = ENEMY_MINE;
                enemies[i] = createEnemy(type);
                continue;
            }

            if (!bonusHeartSpaceshipExists && player.score % 200 <= 50 && player.score >= 200)
            {
                type = ENEMY_HEALTH;
                enemies[i] = createEnemy(type);
                bonusHeartSpaceshipExists = 1;
                continue;
            }

            do
            {
                type = rand() % 4;
            } while ((type == ENEMY_DRONE && droneExists) || (type == ENEMY_BOMBER && bomberExists));

            enemies[i] = createEnemy(type);

            if (type == ENEMY_DRONE)
            {
                droneExists = 1;
            }
            if (type == ENEMY_BOMBER)
            {
                bomberExists = 1;
            }
        }
    }
}

Enemy createEnemy(EnemyType type)
{
    Enemy enemy;
    enemy.type = type;
    int x, width;

    switch (type)
    {
    case ENEMY_MINE:
        width = 40;
        break;
    case ENEMY_FIGHTER:
        width = 100;
        break;
    case ENEMY_DRONE:
        width = 80;
        break;
    case ENEMY_BOMBER:
        width = 100;
        break;
    case ENEMY_HEALTH:
        width = 80;
        break;
    }

    x = findAvailableSpawnSpace(width);

    switch (type)
    {
    case ENEMY_MINE:
        enemy.object = createGameObject(MINE, x, -1 * width, width, width);
        enemy.health = 1;
        break;
    case ENEMY_FIGHTER:
        enemy.object = createGameObject(FIGHTER, x, -1 * width, width, width);
        enemy.health = 3;
        enemy.shootCooldown = player.score > 1500 ? 1000 : 1400; // Shoot faster after score 1500
        enemy.lastShootTime = getTicks();
        break;
    case ENEMY_DRONE:
        enemy.object = createGameObject(DRONE, x, -1 * width, width, width);
        enemy.health = 2;
        break;
    case ENEMY_BOMBER:
        enemy.object = createGameObject(BOMBER, x, -1 * width, width, width);
        enemy.health = 5;
        enemy.shootCooldown = player.score > 2000 ? 1500 : 2000; // Shoot faster after score 2000
        enemy.lastShootTime = getTicks();
        break;
    case ENEMY_HEALTH:
        enemy.object = createGameObject(HEALTH_SPACESHIP, x, -1 * width, width, width);
        enemy.health = 2;
        break;
    }

    return enemy;
}

int checkCollision(GameObject *a, GameObject *b)
{
    int offsetY = 30;

    if (a == NULL || b == NULL)
        return 0;

    return (a->x < b->x + b->width &&
            a->x + a->width > b->x &&
            a->y + offsetY < b->y + b->height &&
            a->y + a->height > b->y + offsetY)
               ? 1
               : 0;
}

void handleCollisions()
{
    // Player's missiles with enemies
    for (int i = 0; i < MAX_PLAYER_MISSILES; i++)
    {
        for (int j = 0; j < currentMaxEnemies; j++)
        {
            if (checkCollision(playerMissiles[i].object, enemies[j].object))
            {
                enemies[j].health--;
                destroyGameObject(playerMissiles[i].object);
                playerMissiles[i].object = NULL;

                if (enemies[j].health <= 0)
                {
                    switch (enemies[j].type)
                    {
                    case ENEMY_MINE:
                        player.score += 30;
                        break;
                    case ENEMY_FIGHTER:
                        player.score += 100;
                        break;
                    case ENEMY_BOMBER:
                        player.score += 150;
                        break;
                    case ENEMY_DRONE:
                        player.score += 60;
                        break;
                    case ENEMY_HEALTH:
                        bonusHeart = createGameObject(FULL_HEART, enemies[j].object->x + enemies[j].object->width / 2 - 20, enemies[j].object->y + enemies[j].object->height / 2 - 20, 40, 40);
                    }

                    createExplosion(enemies[j].object);
                    enemies[j].object = NULL;
                }
            }
        }
    }

    // Enemies' missiles with player
    for (int i = 0; i < MAX_ENEMY_MISSILES; i++)
    {
        if (checkCollision(enemyMissiles[i].object, player.object))
        {
            player.health -= (enemyMissiles[i].type == LARGE_MISSILE ? 1 : 0.5);
            destroyGameObject(enemyMissiles[i].object);
            enemyMissiles[i].object = NULL;
        }
    }

    // Enemies with player
    for (int i = 0; i < currentMaxEnemies; i++)
    {
        if (checkCollision(enemies[i].object, player.object))
        {
            switch (enemies[i].type)
            {
            case ENEMY_MINE:
                player.health -= 0.5;
                break;
            case ENEMY_FIGHTER:
                player.health -= 1;
                break;
            case ENEMY_BOMBER:
                player.health -= 1.5;
                break;
            case ENEMY_DRONE:
                player.health -= 1.5;
                break;
            case ENEMY_HEALTH:
                player.health -= 0.5;
                break;
            }

            createExplosion(enemies[i].object);
            enemies[i].object = NULL;
        }
    }

    // Player with bonus heart
    if (checkCollision(player.object, bonusHeart))
    {
        bonusHeart = NULL;
        player.health += (rand() % 2 + 1) * 0.5;
    }

    if (player.health <= 0)
    {
        createExplosion(player.object);
    }
}

int isPositionOccupied(int x, int width)
{
    for (int i = 0; i < currentMaxEnemies; i++)
    {
        if (enemies[i].object == NULL)
            continue;

        int enemyX = enemies[i].object->x;
        int enemyWidth = enemies[i].object->width;
        if (x < enemyX + enemyWidth && x + width > enemyX)
        {
            return 1;
        }
    }
    return 0;
}

int findAvailableSpawnSpace(int width)
{
    int maxAttempts = 100;
    int attempts = 0;
    int x;

    do
    {
        x = rand() % (WINDOW_WIDTH - width);
        attempts++;
    } while (isPositionOccupied(x, width) && attempts <= maxAttempts);

    return x;
}

void createExplosion(GameObject *gameObject)
{
    Explosion explosion;
    explosion.object = createGameObject(GET_EXPLOSION(rand() % 3 + 1), gameObject->x - 20, gameObject->y - 20, gameObject->width + 20, gameObject->height + 20);
    explosion.startTime = SDL_GetTicks();
    explosion.duration = EXPLOSION_DURATION;
    explosion.opacity = 1.0f; // Full opacity

    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (explosions[i].object == NULL)
        {
            explosions[i] = explosion;
            break;
        }
    }

    destroyGameObject(gameObject);
}