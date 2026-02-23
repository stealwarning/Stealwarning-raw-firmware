#include "ui.h"
#include "theme.h"
#include "screens.h"

Preferences prefs;
int currentBrightness = 150;
int currentThemeIndex = 0;

void saveSettings() {
    prefs.begin("stealwarning", false);
    prefs.putInt("brightness", currentBrightness);
    prefs.putInt("theme", currentThemeIndex);
    prefs.end();
}

void loadSettings() {
    prefs.begin("stealwarning", false);
    currentBrightness = prefs.getInt("brightness", 150);
    currentThemeIndex = prefs.getInt("theme", 0);
    prefs.end();
    applyThemeByIndex(currentThemeIndex);
}

void applyThemeByIndex(int index) {
    currentThemeIndex = index;
    currentTheme = allThemes[index];
}

void drawStatusBar() {
    tft.drawFastHLine(0, 22, SCREEN_WIDTH, currentTheme.statusBar);
    tft.setTextColor(currentTheme.text);
    tft.setTextSize(1);
    tft.setCursor(10, 7);
    
    tft.print(F("STEALWARNING")); 
    if (currentScreen == SETTINGS_MENU) tft.print(F(" > SETTINGS"));
    else if (currentScreen == KEYBOARD_MENU) tft.print(F(" > KEYBOARD"));
}

void setBrightness(int value) {
    currentBrightness = value;
    ledcWrite(0, currentBrightness); 
}

struct tm timeinfo;

void setupTime() {
    timeinfo.tm_year = 2026 - 1900; 
    timeinfo.tm_mon = 0;
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 12;
    timeinfo.tm_min = 0;
    
    time_t t = mktime(&timeinfo);
    struct timeval now = { .tv_sec = t };
    settimeofday(&now, NULL);
}

void updateSystemTime() {
    time_t now;
    time(&now);
    localtime_r(&now, &timeinfo);
}