#pragma once
#include <Arduino.h>

struct Theme {
    uint16_t background;
    uint16_t statusBar;
    uint16_t text;
    uint16_t icon;
    uint16_t backButton;
};

extern Theme currentTheme;
extern Theme themeGreen;
extern Theme themeRed;
extern Theme themeBlue;
extern Theme themeWhite;

void setTheme(Theme t);