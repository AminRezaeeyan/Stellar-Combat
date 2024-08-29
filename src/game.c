#include "game.h"

GameObject *player;
GameObject *fullHeart = NULL;
GameObject *halfHeart = NULL;
GameObject *enemies[10];
GameObject *bullets[10];

float health = 5.0;
int score = 0;

int backgroundY1 = 0;
int backgroundY2 = -WINDOW_HEIGHT;

int setUpGame()
{
    if (initGraphics() != 0 || initLogger("../error.log") != 0)
    {
        return -1;
    }
    atexit(closeGame);

    player = createGameObject(SPACESHIP, 340, 640, 120, 120);
    fullHeart = createGameObject(FULL_HEART, 0, 0, 36, 36);
    halfHeart = createGameObject(HALF_HEART, 0, 0, 18, 36);

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
    displayGuide();

    Uint32 startTicks, endTicks, frameTime;

    int gameRunning = 1;
    while (gameRunning)
    {

        startTicks = getTicks();

        // Process input
        gameRunning = handleEvents();

        // Update
        updateGame();

        // Render
        clearScreen(COLOR_WHITE);
        renderGame();
        presentScreen();

        endTicks = getTicks();
        frameTime = endTicks - startTicks;

        if (FRAME_DELAY > frameTime)
        {
            delay(FRAME_DELAY - frameTime);
        }
    }
}

int handleEvents()
{
    static Uint32 lastBulletTime = 0;
    static const Uint32 bulletCooldown = BULLET_COOLDOWN; // Cooldown time in milliseconds

    EventType event = pollEvent();
    switch (event)
    {
    case EVENT_KEY_ESC:
        return 0;
    case EVENT_QUIT:
        exit(0);
    case EVENT_KEY_UP:
    case EVENT_KEY_W:
        if (player->y > 0)
            moveGameObject(player, 0, -2);
        break;
    case EVENT_KEY_DOWN:
    case EVENT_KEY_S:
        if (player->y + player->height < WINDOW_HEIGHT)
            moveGameObject(player, 0, 2);
        break;
    case EVENT_KEY_LEFT:
    case EVENT_KEY_A:
        if (player->x > 0)
            moveGameObject(player, -5, 0);
        break;
    case EVENT_KEY_RIGHT:
    case EVENT_KEY_D:
        if (player->x + player->width < WINDOW_WIDTH)
            moveGameObject(player, 5, 0);
        break;
    case EVENT_KEY_SPACE:
        if (getTicks() - lastBulletTime >= bulletCooldown)
        {
            for (int i = 0; i < 10; i++)
            {
                if (bullets[i] == NULL)
                {
                    bullets[i] = createGameObject(BULLET, player->x + player->width / 2 - 5, player->y - 25, 10, 50);
                    lastBulletTime = getTicks();
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
void updateGame()
{
    moveBackground();
    updateScoreByInterval();

    for (int i = 0; i < 10; i++)
    {
        if (enemies[i] != NULL)
        {
            moveGameObject(enemies[i], 0, 1);
        }
    }

    for (int i = 0; i < 10; i++)
    {
        if (bullets[i] != NULL)
        {
            moveGameObject(bullets[i], 0, -5);

            if (bullets[i]->y + bullets[i]->height <= 0)
            {
                bullets[i] = NULL;
            }
        }
    }
}

void moveBackground()
{
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
}

void renderMovingBackground()
{
    renderBackground(0, backgroundY1);
    renderBackground(0, backgroundY2);
}

void renderGame()
{
    renderMovingBackground();

    for (int i = 0; i < 10; i++)
    {
        if (enemies[i] != NULL)
        {
            renderGameObject(enemies[i]);
        }
    }

    for (int i = 0; i < 10; i++)
    {
        if (bullets[i] != NULL)
        {
            renderGameObject(bullets[i]);
        }
    }

    renderGameObject(player);
    renderHealth();
    renderScore();
}

void renderHealth()
{
    int fullHearts = (int)health;
    int halfHearts = (health - fullHearts >= 0.5) ? 1 : 0;

    int x = 10;
    int y = 10;

    // Render full hearts
    for (int i = 0; i < fullHearts; i++)
    {
        fullHeart->x = x;
        fullHeart->y = y;
        renderGameObject(fullHeart);
        x += 38;
    }

    // Render half heart
    if (halfHearts > 0)
    {
        halfHeart->x = x;
        halfHeart->y = y;
        renderGameObject(halfHeart);
        x += 38; // Move to the next position
    }
}

void renderScore()
{
    int digitsCount = countDigits(score);
    char scoreString[10];
    sprintf(scoreString, "%d", score);
    renderText(scoreString, FONT_BANGERS_LG, COLOR_WHITE, WINDOW_WIDTH - 20 * digitsCount - 5, 10);
}

void updateScoreByInterval()
{
    static Uint32 lastUpdateTime = 0;
    static const Uint32 updateInterval = 1000; // miliseconds

    Uint32 currentTime = getTicks();
    if (currentTime - lastUpdateTime >= updateInterval)
    {
        score += 5;
        lastUpdateTime = currentTime;
    }
}

void displayGuide()
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

void pause()
{
    renderText("Pause", FONT_BANGERS_LG, COLOR_RED, 350, 10);
    presentScreen();
    waitForKey();
}