#include "theme.h"
#include "config.h"

Theme themeGreen = {TFT_BLACK, TFT_DARKGREEN, TFT_GREEN, TFT_GREEN, TFT_WHITE};
Theme themeRed = {TFT_BLACK, TFT_MAROON, TFT_RED, TFT_RED, TFT_WHITE};
Theme themeBlue = {TFT_BLACK, TFT_NAVY, TFT_CYAN, TFT_CYAN, TFT_WHITE};
Theme themeWhite = {
    TFT_BLACK,    
    TFT_LIGHTGREY,
    TFT_WHITE,    
    TFT_WHITE,     
    TFT_DARKGREY  
};

Theme currentTheme = themeGreen;

void setTheme(Theme t) {
    currentTheme = t;
}