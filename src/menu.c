#include "menu.h"

void renderMenu(MenuOption selectedOption)
{
    const char *options[MENU_OPTION_COUNT] = {"Play", "Records", "Exit"};
    Font font = FONT_BREE_SERIF_LG;
    Color normalColor = COLOR_WHITE;
    Color selectedColor = COLOR_GOLD;

    for (int i = 0; i < MENU_OPTION_COUNT; ++i)
    {
        Color color = (i == selectedOption) ? selectedColor : normalColor;
        renderText(options[i], font, color, 50 + i * 60, 250 + i * 140);
    }

    presentScreen();
}

MenuOption runMenu()
{
    GameObject *logo = createGameObject("../resources/assets/images/logo.png", WINDOW_WIDTH - 450, 0, 450, 450);
    renderGameObject(logo);

    renderText("Developed by: Amin Rezaeeyan", FONT_BREE_SERIF_SM, COLOR_WHITE, 10, WINDOW_HEIGHT - 50);
    renderText("Use Arrow Keys or WASD to Navigate", FONT_BREE_SERIF_SM, COLOR_WHITE, WINDOW_WIDTH - 310, WINDOW_HEIGHT - 50);

    MenuOption selectedOption = MENU_PLAY;
    int menuRunning = 1;

    while (menuRunning)
    {
        renderMenu(selectedOption);
        presentScreen();

        EventType event = pollEvent();
        switch (event)
        {
        case EVENT_QUIT:
            return MENU_EXIT;
        case EVENT_KEY_UP:
        case EVENT_KEY_W:
            if (selectedOption > 0)
                selectedOption--;
            break;
        case EVENT_KEY_DOWN:
        case EVENT_KEY_S:
            if (selectedOption < MENU_OPTION_COUNT - 1)
                selectedOption++;
            break;
        case EVENT_KEY_ENTER:
        case EVENT_KEY_SPACE:
            menuRunning = 0;
            break;
        default:
            break;
        }
    }
    return selectedOption;
}