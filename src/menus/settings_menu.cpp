#include "settings_menu.h"
#include "config.h"
#include "theme.h"
#include "screens.h"
#include "ui.h"
#include <Preferences.h>
#include "datetime_menu.h"

void updateBrightnessUI(bool fullRedraw);
void resetToFactory();

SettingsPage currentPage = CATEGORIES;

const char* const categoryLabels[] = {
    "WIFI", "DATE & TIME", "THEME", "BRIGHTNESS", "RESET ALL"
};
const int categoryCount = sizeof(categoryLabels) / sizeof(categoryLabels[0]);

void drawSettingsMenu() {
    tft.fillScreen(currentTheme.background);
    drawStatusBar();
    drawSidebar();

    tft.setTextColor(currentTheme.text, currentTheme.background);
    tft.setTextSize(2);

    if (currentPage == CATEGORIES) {
        for (int i = 0; i < categoryCount; i++) {
            tft.setCursor(70, 55 + (i * 35));
            tft.print(F("> ")); 
            tft.print(categoryLabels[i]);
        }
    }
    else if (currentPage == THEME_SELECT) {
        tft.setCursor(70, 45); 
        tft.print(F("SELECT THEME:"));

        int size = 60, gap = 30;
        
        for (int i = 0; i < 4; i++) {
            int x = 85 + (i % 2) * (size + gap);
            int y = 80 + (i / 2) * (size + gap);
            
            tft.fillRect(x, y, size, size, allThemes[i].icon); 
            
            if (currentThemeIndex == i) {
                tft.drawRect(x - 3, y - 3, size + 6, size + 6, currentTheme.text);
            }
        }
    }
    else if (currentPage == BRIGHTNESS_SELECT) {
        updateBrightnessUI(true);
    }
}

void updateBrightnessUI(bool fullRedraw) {
    int startX = 70, sliderY = 120;
    int percent = map(currentBrightness, 0, 255, 0, 100);

    if (fullRedraw) {
        tft.setCursor(startX, 45); tft.print(F("BRIGHTNESS:"));
        tft.drawRoundRect(startX, sliderY, 200, 30, 5, currentTheme.text);
        tft.drawRoundRect(startX, 180, 60, 40, 8, currentTheme.backButton);
        tft.setCursor(startX + 22, 192); tft.print(F("-"));
        tft.drawRoundRect(210, 180, 60, 40, 8, currentTheme.backButton);
        tft.setCursor(232, 192); tft.print(F("+"));
    }

    tft.fillRect(150, 80, 60, 25, currentTheme.background);
    tft.setCursor(150, 80);
    tft.print(percent); tft.print(F("%"));

    int fillW = map(currentBrightness, 0, 255, 0, 200);
    tft.fillRect(startX + 2, sliderY + 2, 196, 26, currentTheme.background);
    tft.fillRoundRect(startX, sliderY, fillW, 30, 5, currentTheme.icon);
    tft.drawRoundRect(startX, sliderY, 200, 30, 5, currentTheme.text);
}

void handleSettingsMenuTouch(int x, int y) {
    if (currentPage == CATEGORIES) {
        int index = (y - 50) / 35; 
        if (index >= 0 && index < categoryCount) {
            switch(index) {
                case 0: changeScreen(WIFI_MENU); break;
                case 1: changeScreen(DATETIME_MENU); break;
                case 2: currentPage = THEME_SELECT; drawSettingsMenu(); break;
                case 3: currentPage = BRIGHTNESS_SELECT; drawSettingsMenu(); break;
                case 4: resetToFactory(); break;
            }
        }
    }
    else if (currentPage == THEME_SELECT) {
        int themeIdx = -1;
        if (y > 70 && y < 145) themeIdx = (x < 155) ? 0 : 1;
        else if (y > 155 && y < 230) themeIdx = (x < 155) ? 2 : 3;

        if (themeIdx != -1) {
            applyThemeByIndex(themeIdx);
            saveSettings();
            drawSettingsMenu();
        }
    }
    else if (currentPage == BRIGHTNESS_SELECT) {
        if (y > 175 && y < 225) {
            if (x > 70 && x < 130) setBrightness(max(5, currentBrightness - 25));
            else if (x > 210 && x < 270) setBrightness(min(255, currentBrightness + 25));
            updateBrightnessUI(false);
        } else {
            handleBrightnessSlider(x, y);
        }
    }
}

void handleBrightnessSlider(int x, int y) {
    if (currentPage == BRIGHTNESS_SELECT && y > 100 && y < 160 && x > 70 && x < 270) {
        int newVal = constrain(map(x, 70, 270, 0, 255), 5, 255);
        if (abs(newVal - currentBrightness) > 2) {
            setBrightness(newVal); 
            updateBrightnessUI(false); 
        }
    }
}

void resetToFactory() {
    prefs.begin("stealwarning", false);
    prefs.clear();
    prefs.end();
    ESP.restart();
}