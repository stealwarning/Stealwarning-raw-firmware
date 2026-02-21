#ifndef SETTINGS_MENU_H
#define SETTINGS_MENU_H

#include <Arduino.h>

enum SettingsPage { CATEGORIES, THEME_SELECT, BRIGHTNESS_SELECT };

extern SettingsPage currentPage;

void factoryReset();

void drawSettingsMenu();
void handleSettingsMenuTouch(int x, int y);
void handleBrightnessSlider(int x, int y); 

#endif