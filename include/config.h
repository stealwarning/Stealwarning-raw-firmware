#pragma once
#include <TFT_eSPI.h>

#define TFT_BL 32
#define TFT_DC 22

#include <Preferences.h>

extern Preferences prefs;
extern int currentBrightness;
void setBrightness(int value);
void saveSettings();
void loadSettings();

extern int currentThemeIndex; // 0: Green, 1: Red, 2: Blue, 3: White
void applyThemeByIndex(int index);

extern TFT_eSPI tft;

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

extern uint16_t accentColor;