#ifndef SCREENS_H
#define SCREENS_H

#include <Arduino.h>

enum ScreenState { MAIN_MENU, SETTINGS_MENU, KEYBOARD_MENU, DATETIME_MENU, FILE_EXPLORER, WIFI_MENU };
extern ScreenState currentScreen;
extern ScreenState previousScreen;

enum KeyboardTarget { 
    KBD_NONE, 
    KBD_WIFI_PASS, 
    KBD_FILE_NAME,
    KBD_GENERAL_TEXT
};

extern String selectedSSID;
extern int wifiOffset;
extern KeyboardTarget currentKbdTarget;
extern String inputBuffer;

void drawSidebar();
void changeScreen(ScreenState newScreen); 
void drawCurrentScreen();
void processAllTouches(int x, int y, bool isMoving);

#endif