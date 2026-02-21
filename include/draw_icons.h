#ifndef DRAW_ICONS_H
#define DRAW_ICONS_H

#include <TFT_eSPI.h>
#include "ui.h"
#include "theme.h"

inline void drawIconSettings(int x, int y, int size, uint16_t color) {
    int cx = x + size/2;
    int cy = y + size/2;
    
    for (int i = 0; i < 360; i += 45) {
        float rad = (i + 22) * DEG_TO_RAD;
        int x1 = cx + cos(rad) * (size/2 - 10);
        int y1 = cy + sin(rad) * (size/2 - 10);
        int x2 = cx + cos(rad) * (size/2);
        int y2 = cy + sin(rad) * (size/2);
        
        for(int thick = -3; thick <= 3; thick++) {
            tft.drawLine(x1 + thick, y1, x2 + thick, y2, color);
            tft.drawLine(x1, y1 + thick, x2, y2 + thick, color);
        }
    }

    tft.fillCircle(cx, cy, size/3, color);
    tft.fillCircle(cx, cy, size/6, currentTheme.background);
}

inline void drawIconKeyboard(int x, int y, int size, uint16_t color) {
    int w = size;
    int h = size * 0.75;
    int ky_start = y + (size - h) / 2;

    for(int i = 0; i < 3; i++) {
        tft.drawRoundRect(x + i, ky_start + i, w - i*2, h - i*2, 4, color);
    }

    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 4; col++) {
            tft.fillRect(x + 8 + col * 9, ky_start + 8 + row * 7, 6, 4, color);
        }
    }
    
    tft.fillRect(x + 12, ky_start + h - 10, w - 24, 4, color);
}

inline void drawIconClock(int x, int y, int size, uint16_t color) {
    int cx = x + size/2;
    int cy = y + size/2;
    int r = size/2 - 2;

    for(int i=0; i<3; i++) tft.drawCircle(cx, cy, r-i, color);
    
    tft.fillRect(cx - 1, cy - r + 8, 3, r - 8, color);
    tft.fillRect(cx, cy - 1, r - 8, 3, color);        
}



inline void drawIconExplorer(int x, int y, int size, uint16_t color) {
    int w = size;
    int h = size * 0.8;
    tft.fillRect(x, y + 5, w, h - 5, color); 
    tft.fillRect(x, y, w * 0.4, 6, color);   
    tft.fillRect(x + 2, y + 7, w - 4, h - 9, currentTheme.background); 
}

inline void drawSmallFolder(int x, int y, uint16_t color) {
    tft.fillRect(x, y + 2, 14, 10, color);
    tft.fillRect(x, y, 6, 3, color);      
}

inline void drawSmallFile(int x, int y, uint16_t color) {
    tft.drawRect(x, y, 12, 14, color);
    tft.drawLine(x + 8, y, x + 12, y + 4, color);
}



#endif