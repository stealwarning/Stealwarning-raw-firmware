#include "theme.h"
#include "TFT_eSPI.h"

const Theme allThemes[] = {
    {TFT_BLACK, TFT_DARKGREEN, TFT_GREEN, TFT_GREEN, TFT_WHITE}, // Green
    {TFT_BLACK, TFT_MAROON, TFT_RED, TFT_RED, TFT_WHITE},        // Red
    {TFT_BLACK, TFT_NAVY, TFT_CYAN, TFT_CYAN, TFT_WHITE},        // Blue
    {TFT_BLACK, TFT_LIGHTGREY, TFT_WHITE, TFT_WHITE, TFT_DARKGREY} // White
};

Theme currentTheme = allThemes[0];

void setTheme(Theme t) {
    currentTheme = t;
}