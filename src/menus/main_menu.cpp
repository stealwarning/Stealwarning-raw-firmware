#include "config.h"
#include "theme.h"
#include "screens.h"
#include "ui.h"
#include "draw_icons.h"

struct MenuOption {
    const char* label;
    void (*drawIcon)(int, int, int, uint16_t);
    ScreenState targetScreen;
};

const MenuOption options[] = {
    {"Settings", drawIconSettings, SETTINGS_MENU},
    {"Keyboard", drawIconKeyboard, KEYBOARD_MENU},
    {"Explorer", drawIconExplorer, FILE_EXPLORER}
};

const int totalOptions = sizeof(options) / sizeof(options[0]);
int selectedOption = 0;

void drawMainMenu() {
    tft.fillScreen(currentTheme.background);
    drawStatusBar();

    tft.setTextColor(currentTheme.icon);
    tft.setTextSize(4);
    tft.setCursor(20, 110); tft.print(F("<"));
    tft.setCursor(280, 110); tft.print(F(">"));

    options[selectedOption].drawIcon(136, 90, 48, currentTheme.icon);
    
    tft.setTextColor(currentTheme.text);
    tft.setTextSize(2);
    
    const char* label = options[selectedOption].label;
    int labelX = 160 - (strlen(label) * 6); 
    tft.setCursor(labelX, 185);
    tft.print(label);
}

void handleMainMenuTouch(int x, int y) {
    if (y > 40 && y < 180) {
        if (x < 80) {
            selectedOption = (selectedOption - 1 + totalOptions) % totalOptions;
            drawMainMenu();
        }
        else if (x > 240) {
            selectedOption = (selectedOption + 1) % totalOptions;
            drawMainMenu();
        }
        else if (x > 100 && x < 220) {
            changeScreen(options[selectedOption].targetScreen); 
        }
    }
}