#pragma once
#include <TFT_eSPI.h>
#include <Preferences.h>

#define TFT_BL 32
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

#define TOUCH_X_MIN 240
#define TOUCH_X_MAX 3800
#define TOUCH_Y_MIN 240
#define TOUCH_Y_MAX 3800

extern Preferences prefs;
extern int currentBrightness;
extern int currentThemeIndex;

void setBrightness(int value);
void saveSettings();
void loadSettings();
void applyThemeByIndex(int index);

extern TFT_eSPI tft;