#include "ui.h"
#include "theme.h"
#include "screens.h"
#include <Preferences.h>
#include "config.h"

Preferences prefs;
int currentBrightness = 150;

int currentThemeIndex = 0;

void saveSettings() {
    prefs.begin("stealwarning", false);
    prefs.putInt("brightness", currentBrightness);
    prefs.putInt("theme", (int)currentThemeIndex);
    prefs.end();
    Serial.println("Settings Saved!");
}

void loadSettings() {
    prefs.begin("stealwarning", false);
    currentBrightness = prefs.getInt("brightness", 150);
    currentThemeIndex = prefs.getInt("theme", 0);
    
    long savedEpoch = prefs.getLong("epoch", 1735689600);
    struct timeval tv = { .tv_sec = savedEpoch };
    settimeofday(&tv, NULL);
    
    prefs.end();
    applyThemeByIndex(currentThemeIndex);
}

void applyThemeByIndex(int index) {
    currentThemeIndex = index;
    if (index == 0) currentTheme = themeGreen;
    else if (index == 1) currentTheme = themeRed;
    else if (index == 2) currentTheme = themeBlue;
    else if (index == 3) currentTheme = themeWhite;
}

void setBrightness(int value) {
    currentBrightness = value;
    ledcWrite(0, currentBrightness); 
}


void drawStatusBar() {
    tft.drawFastHLine(0, 22, SCREEN_WIDTH, currentTheme.statusBar);
    
    tft.setTextColor(currentTheme.text);
    tft.setTextSize(1);
    tft.setCursor(10, 7);
    
    if (currentScreen == SETTINGS_MENU) tft.print("STEALWARNING > SETTINGS");
    else if (currentScreen == KEYBOARD_MENU) tft.print("STEALWARNING > KEYBOARD");
    else tft.print("STEALWARNING");
}

void drawNavigationButtons() {
    tft.drawRoundRect(5, 30, 45, 35, 4, currentTheme.backButton);
    tft.setTextColor(currentTheme.backButton);
    tft.setTextSize(2);
    tft.setCursor(15, 40);
    tft.print("<-");

    tft.drawRoundRect(5, 75, 45, 35, 4, currentTheme.backButton);
    tft.setCursor(18, 85);
    tft.print("H"); 
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