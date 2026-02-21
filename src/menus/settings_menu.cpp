#include "settings_menu.h"
#include "config.h"
#include "theme.h"
#include "screens.h"
#include "ui.h"
#include <Preferences.h>
#include "datetime_menu.h"

SettingsPage currentPage = CATEGORIES;

void drawSettingsMenu() {
    tft.fillScreen(currentTheme.background);
    drawStatusBar();
    drawNavigationButtons();

    int startX = 70;
    tft.setTextColor(currentTheme.text, currentTheme.background);
    tft.setTextSize(2);

    if (currentPage == CATEGORIES) {
        tft.setCursor(startX, 55);  tft.print("> WIFI");
        tft.setCursor(startX, 90);  tft.print("> DATE & TIME");
        tft.setCursor(startX, 125); tft.print("> THEME");
        tft.setCursor(startX, 160); tft.print("> BRIGHTNESS");
        tft.setCursor(startX, 195); tft.print("> RESET ALL");
    }
    
    else if (currentPage == THEME_SELECT) {
        tft.setCursor(70, 45); 
        tft.print("SELECT THEME:");

        int size = 60;    
        int gap = 30;     
        int x1 = 85, x2 = 85 + size + gap;
        int y1 = 80, y2 = 80 + size + gap;

        tft.fillRect(x1, y1, size, size, themeGreen.icon);  
        tft.fillRect(x2, y1, size, size, themeRed.icon);    
        tft.fillRect(x1, y2, size, size, themeBlue.icon);   
        tft.fillRect(x2, y2, size, size, TFT_WHITE);        

        int selX = (currentThemeIndex % 2 == 0) ? x1 : x2;
        int selY = (currentThemeIndex < 2) ? y1 : y2;
        tft.drawRect(selX - 3, selY - 3, size + 6, size + 6, currentTheme.text);
    }

    else if (currentPage == BRIGHTNESS_SELECT) {
        tft.setTextColor(currentTheme.text, currentTheme.background);
        tft.setCursor(startX, 45); tft.print("BRIGHTNESS:");
        
        tft.setCursor(150, 80); 
        tft.print(String(map(currentBrightness, 0, 255, 0, 100)) + "%");

        int sliderY = 120;
        tft.drawRoundRect(startX, sliderY, 200, 30, 5, currentTheme.text); 
        int fillW = map(currentBrightness, 0, 255, 0, 200);
        tft.fillRoundRect(startX, sliderY, fillW, 30, 5, currentTheme.icon); 

        tft.drawRoundRect(startX, 180, 60, 40, 8, currentTheme.backButton);
        tft.setCursor(startX + 22, 192); tft.print("-");

        tft.drawRoundRect(210, 180, 60, 40, 8, currentTheme.backButton);
        tft.setCursor(210 + 22, 192); tft.print("+");
    }
}

void resetToFactory() {
    prefs.begin("stealwarning", false);
    prefs.clear();
    prefs.end();
    
    setBrightness(150);
    setTheme(themeGreen);
    
    ESP.restart(); 
}

void updateBrightnessUI() {
    int startX = 70;
    int sliderY = 120;
    
    tft.fillRect(150, 80, 60, 25, currentTheme.background); 
    tft.setTextColor(currentTheme.text, currentTheme.background);
    tft.setCursor(150, 80); 
    tft.print(String(map(currentBrightness, 0, 255, 0, 100)) + "%");

    tft.fillRect(startX + 2, sliderY + 2, 196, 26, currentTheme.background);
    int fillW = map(currentBrightness, 0, 255, 0, 200);
    tft.fillRoundRect(startX, sliderY, fillW, 30, 5, currentTheme.icon);
    tft.drawRoundRect(startX, sliderY, 200, 30, 5, currentTheme.text);
}

void handleSettingsMenuTouch(int x, int y) {
    if (currentPage == CATEGORIES) {
        if (y > 50 && y < 85) changeScreen(WIFI_MENU);
        else if (y > 85 && y < 120) changeScreen(DATETIME_MENU);
        else if (y > 120 && y < 155) { currentPage = THEME_SELECT; drawSettingsMenu(); }
        else if (y > 155 && y < 190) { currentPage = BRIGHTNESS_SELECT; drawSettingsMenu(); }
        else if (y > 190 && y < 230) resetToFactory();
    }
    
    else if (currentPage == THEME_SELECT) {
        if (y > 70 && y < 145) { 
            if (x > 70 && x < 155) applyThemeByIndex(0);      
            else if (x > 165 && x < 250) applyThemeByIndex(1);
        } 
        else if (y > 155 && y < 230) {
            if (x > 70 && x < 155) applyThemeByIndex(2);      
            else if (x > 165 && x < 250) applyThemeByIndex(3);
        }
        saveSettings();
        drawSettingsMenu();
    }

    else if (currentPage == BRIGHTNESS_SELECT) {
        if (y > 100 && y < 160 && x > 70 && x < 270) {
            int newVal = map(x, 70, 270, 0, 255);
            newVal = constrain(newVal, 5, 255);
            if (abs(newVal - currentBrightness) > 2) {
                setBrightness(newVal); 
                updateBrightnessUI(); 
            }
        }
        if (y > 175 && y < 225) {
            if (x > 70 && x < 130) { setBrightness(max(5, currentBrightness - 25)); drawSettingsMenu(); }
            if (x > 210 && x < 270) { setBrightness(min(255, currentBrightness + 25)); drawSettingsMenu(); }
        }
    }
}

void handleBrightnessSlider(int x, int y) {
    if (currentPage == BRIGHTNESS_SELECT) {
        if (y > 100 && y < 160 && x > 70 && x < 270) {
            int newVal = map(x, 70, 270, 0, 255);
            newVal = constrain(newVal, 5, 255);

            if (abs(newVal - currentBrightness) > 2) {
                setBrightness(newVal); 
                int fillW = map(currentBrightness, 0, 255, 0, 200);
                tft.fillRect(70 + fillW, 122, 200 - fillW, 26, currentTheme.background);
                tft.fillRoundRect(70, 120, fillW, 30, 5, currentTheme.icon);
                tft.setTextColor(currentTheme.text, currentTheme.background);
                tft.setCursor(150, 80);
                tft.print(String(map(currentBrightness, 0, 255, 0, 100)) + "%  ");
            }
        }
    }
}