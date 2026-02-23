#include "screens.h"
#include "menus/main_menu.h"
#include "menus/settings_menu.h"
#include "menus/keyboard_menu.h"
#include "menus/datetime_menu.h"
#include "menus/file_explorer.h"
#include "menus/wifi_menu.h"
#include "theme.h"
#include "ui.h"
#include <WiFi.h>

ScreenState currentScreen = MAIN_MENU;
ScreenState previousScreen = MAIN_MENU;
KeyboardTarget currentKbdTarget = KBD_NONE;
String selectedSSID = "";
int wifiOffset = 0;
unsigned long lastScreenChange = 0;

void drawSidebar() {
    tft.drawFastVLine(60, 0, 240, currentTheme.text);
    
    tft.drawRoundRect(5, 35, 50, 45, 5, currentTheme.backButton); 
    tft.setTextColor(currentTheme.backButton);
    tft.setTextSize(2);
    tft.setCursor(20, 47);
    tft.print(F("<-"));

    tft.drawRoundRect(5, 90, 50, 45, 5, currentTheme.icon);
    tft.setTextColor(currentTheme.icon);
    tft.setCursor(22, 102);
    tft.print(F("H")); 
}

void drawCurrentScreen() {
    if (currentScreen != MAIN_MENU) drawSidebar();

    switch (currentScreen) {
        case MAIN_MENU:     drawMainMenu(); break;
        case SETTINGS_MENU: drawSettingsMenu(); break;
        case KEYBOARD_MENU: drawKeyboard(); break;
        case DATETIME_MENU: drawDateTimeMenu(); break;
        case FILE_EXPLORER: drawFileExplorer(currentPath.c_str()); break;
        case WIFI_MENU:     drawWifiMenu(); break;
    }
}

void processAllTouches(int x, int y, bool isMoving) {
    if (currentScreen != MAIN_MENU && !isMoving && x < 60) {
        if (y > 35 && y < 80) {
            switch (currentScreen) {
                case WIFI_MENU:
                case DATETIME_MENU: 
                    changeScreen(SETTINGS_MENU); break;
                case SETTINGS_MENU:
                    if (currentPage != CATEGORIES) { 
                        currentPage = CATEGORIES; drawSettingsMenu(); 
                    } else changeScreen(MAIN_MENU); 
                    break;
                case FILE_EXPLORER: {
                    if (currentPath != "/") {
                        int lastSlash = currentPath.lastIndexOf('/');
                        currentPath = currentPath.substring(0, max(1, lastSlash));
                        drawFileExplorer(currentPath.c_str());
                    } else changeScreen(MAIN_MENU);
                    break;
                }
                default: changeScreen(MAIN_MENU); break;
            }
            return;
        }
        
        if (y > 90 && y < 135) {
            changeScreen(MAIN_MENU);
            return;
        }
    }

    if (isMoving) {
        if (currentScreen == SETTINGS_MENU) handleBrightnessSlider(x, y);
        return; 
    }

    switch (currentScreen) {
        case MAIN_MENU:     handleMainMenuTouch(x, y); break;
        case SETTINGS_MENU: handleSettingsMenuTouch(x, y); break;
        case KEYBOARD_MENU: handleKeyboardMenuTouch(x, y); break;
        case DATETIME_MENU: handleDateTimeTouch(x, y); break;
        case FILE_EXPLORER: handleFileExplorerTouch(x, y); break; 
        case WIFI_MENU:     handleWifiMenuTouch(x, y); break;
    }
}

void changeScreen(ScreenState newScreen) {
    if (millis() - lastScreenChange < 300) return;
    lastScreenChange = millis();

    if (newScreen == KEYBOARD_MENU) previousScreen = currentScreen;
    
    currentScreen = newScreen;
    if (newScreen == SETTINGS_MENU) currentPage = CATEGORIES;

    if (newScreen == WIFI_MENU) {
        wifiOffset = 0;
        WiFi.mode(WIFI_STA);
        if (WiFi.status() != WL_CONNECTED) WiFi.scanNetworks(true);
    }

    tft.fillScreen(currentTheme.background);
    drawCurrentScreen();
}