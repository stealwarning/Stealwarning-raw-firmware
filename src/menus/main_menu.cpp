#include "config.h"
#include "theme.h"
#include "screens.h"
#include "ui.h"
#include "draw_icons.h"

int selectedOption = 0;

void drawMainMenu() {
    tft.fillScreen(currentTheme.background);
    drawStatusBar();

    tft.setTextColor(currentTheme.icon);
    tft.setTextSize(4);
    tft.setCursor(20, 110); tft.print("<");
    tft.setCursor(280, 110); tft.print(">");

    if (selectedOption == 0) {
        drawIconSettings(136, 90, 48, currentTheme.icon);
    } else if (selectedOption == 1) {
        drawIconKeyboard(136, 90, 48, currentTheme.icon);
    } else if (selectedOption == 2) {
        drawIconExplorer(136, 90, 48, currentTheme.icon);
    }
    
    tft.setTextColor(currentTheme.text);
    tft.setTextSize(2);
    
    String label = "";
    if (selectedOption == 0) label = "Settings";
    else if (selectedOption == 1) label = "Keyboard";
    else if (selectedOption == 2) label = "Explorer";

    tft.setCursor(160 - (label.length() * 6), 185);
    tft.print(label);
}

void handleMainMenuTouch(int x, int y) {
    if (x < 80 && y > 40 && y < 180) {
        selectedOption--;
        if (selectedOption < 0) selectedOption = 2;
        drawMainMenu();
    }
    else if (x > 240 && y > 40 && y < 180) {
        selectedOption++;
        if (selectedOption > 2) selectedOption = 0; 
        drawMainMenu();
    }
    else if (x > 100 && x < 220 && y > 40 && y < 180) {
        if (selectedOption == 0) currentScreen = SETTINGS_MENU;
        else if (selectedOption == 1) currentScreen = KEYBOARD_MENU;
        else currentScreen = FILE_EXPLORER;
        drawCurrentScreen();
    }
}
