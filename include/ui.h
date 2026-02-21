#pragma once
#include "config.h"
#ifndef UI_H
#define UI_H

#include <TFT_eSPI.h>
#include <time.h>

extern TFT_eSPI tft;

extern struct tm timeinfo;
void setupTime();
void updateSystemTime();

void drawStatusBar();
void drawMainMenu(int selected);

void drawSettingsMenu();
void drawKeyboardMenu();
void drawNavigationButtons();

void loadSettings();

#endif