#include "datetime_menu.h"
#include "ui.h"
#include "screens.h"
#include "config.h"
#include <sys/time.h>

void drawArrow(int x, int y, bool up, uint16_t color) {
    int w = 30;
    int h = 20;
    if (up) {
        tft.fillTriangle(x, y + h, x + w / 2, y, x + w, y + h, color);
    } else {
        tft.fillTriangle(x, y, x + w / 2, y + h, x + w, y, color);
    }
}

void drawDateTimeMenu() {
    tft.fillScreen(currentTheme.background);
    drawStatusBar();
    drawSidebar();
    updateSystemTime();

    tft.setTextColor(currentTheme.text, currentTheme.background);
    tft.setTextSize(3);
    tft.setCursor(95, 35); 
    tft.print(F("Set Time"));

    int timeY = 110;
    int hourX = 85;
    int minX = 200;

    tft.setTextSize(6);
    tft.setCursor(hourX, timeY);
    if (timeinfo.tm_hour < 10) tft.print("0");
    tft.print(timeinfo.tm_hour);
    tft.print(":");
    if (timeinfo.tm_min < 10) tft.print("0");
    tft.print(timeinfo.tm_min);

    drawArrow(hourX + 5, timeY - 45, true, currentTheme.icon);
    drawArrow(minX + 5, timeY - 45, true, currentTheme.icon);
    drawArrow(hourX + 5, timeY + 65, false, currentTheme.icon);
    drawArrow(minX + 5, timeY + 65, false, currentTheme.icon);
}

void handleDateTimeTouch(int x, int y) {
    bool changed = false;

    if (y > 50 && y < 100) {
        if (x > 70 && x < 130) {
            timeinfo.tm_hour = (timeinfo.tm_hour + 1) % 24;
            changed = true;
        }
        if (x > 190 && x < 250) {
            timeinfo.tm_min = (timeinfo.tm_min + 1) % 60;
            changed = true;
        }
    }
    else if (y > 150 && y < 210) {
        if (x > 70 && x < 130) {
            timeinfo.tm_hour = (timeinfo.tm_hour > 0) ? timeinfo.tm_hour - 1 : 23;
            changed = true;
        }
        if (x > 190 && x < 250) {
            timeinfo.tm_min = (timeinfo.tm_min > 0) ? timeinfo.tm_min - 1 : 59;
            changed = true;
        }
    }

    if (changed) {
        time_t t = mktime(&timeinfo);
        struct timeval now = { .tv_sec = t };
        settimeofday(&now, NULL);
        
        prefs.begin("stealwarning", false);
        prefs.putLong("epoch", (long)t);
        prefs.end();

        drawDateTimeMenu();
    }
}