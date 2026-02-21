#include "menus/keyboard_menu.h"
#include "ui.h"
#include "screens.h"
#include "theme.h"
#include <WiFi.h>
#include "menus/file_explorer.h"

extern KeyboardTarget currentKbdTarget; 
extern String selectedSSID;

enum KbdMode { LOWER, UPPER, SYMBOLS };
KbdMode currentKbdMode = LOWER;
String inputBuffer = ""; 
unsigned long kbdOpenTime = 0; 

const char* keysL = "1234567890qwertyuiopasdfghjklzxcvbnm";
const char* keysU = "1234567890QWERTYUIOPASDFGHJKLZXCVBNM";
const char* keysS = "1234567890!@#$%^&*()-=_+[]{}\\|/?><.,";

const int startX = 75; 
const int keyW = 23;    
const int keyH = 38;    
const int startY = 65;

void updateInputDisplay() {
    tft.fillRect(startX + 2, 12, 185, 41, currentTheme.background); 
    tft.drawRoundRect(startX, 10, 190, 45, 5, currentTheme.text);
    
    tft.setTextColor(currentTheme.text);
    tft.setTextSize(2);
    tft.setCursor(startX + 10, 22);
    tft.print(inputBuffer + "|");

    tft.drawRoundRect(280, 10, 35, 45, 5, 0x07E0);
    tft.setTextColor(0x07E0); tft.setCursor(290, 22); tft.print("V");
}

void drawKeyboard() {
    kbdOpenTime = millis();
    tft.fillScreen(currentTheme.background);
    drawSidebar(); 
    updateInputDisplay();

    const char* currentSet = (currentKbdMode == LOWER) ? keysL : (currentKbdMode == UPPER ? keysU : keysS);

    for (int r = 0; r < 4; r++) {
        int keysInRow = (r < 2) ? 10 : (r == 2 ? 9 : 7);
        int startIndex = (r == 0) ? 0 : (r == 1 ? 10 : (r == 2 ? 20 : 29));

        for (int i = 0; i < keysInRow; i++) {
            int kx = startX + i * (keyW + 2);
            int ky = startY + r * (keyH + 2);
            
            tft.setTextColor(currentTheme.text);
            tft.setTextSize(2);
            tft.setCursor(kx + 6, ky + 11);
            tft.print(currentSet[startIndex + i]);
        }
    }

    tft.setTextSize(2);
    tft.setTextColor(currentTheme.icon);
    tft.setCursor(startX, 215); 
    tft.print(currentKbdMode == LOWER ? "abc" : (currentKbdMode == UPPER ? "ABC" : "#+="));

    tft.setTextColor(currentTheme.text);
    tft.setCursor(startX + 75, 215); 
    tft.print("SPACE");

    tft.setTextColor(0xF800);
    tft.setCursor(startX + 185, 215); 
    tft.print("<<");
}

void handleKeyboardMenuTouch(int x, int y) {
    if (millis() - kbdOpenTime < 300) return;

    if (y < 60 && x > 275) {
        Serial.println("Keyboard DONE. Text: " + inputBuffer);

        switch (currentKbdTarget) {
            case KBD_WIFI_PASS:
                WiFi.begin(selectedSSID.c_str(), inputBuffer.c_str());
                changeScreen(WIFI_MENU); 
                break;

            case KBD_FILE_NAME:
                Serial.println("Saving file: " + inputBuffer);
                changeScreen(FILE_EXPLORER);
                break;

            default:
                changeScreen(previousScreen);
                break;
        }

        currentKbdTarget = KBD_NONE; 
        return;
    }

    if (x >= startX && y >= startY && y < startY + 4 * (keyH + 2)) {
        int row = (y - startY) / (keyH + 2);
        int col = (x - startX) / (keyW + 2);
        int keysInRow = (row < 2) ? 10 : (row == 2 ? 9 : 7);
        
        if (col >= 0 && col < keysInRow) {
            int startIndex = (row == 0) ? 0 : (row == 1 ? 10 : (row == 2 ? 20 : 29));
            const char* currentSet = (currentKbdMode == LOWER) ? keysL : (currentKbdMode == UPPER ? keysU : keysS);
            
            if (inputBuffer.length() < 16) {
                inputBuffer += currentSet[startIndex + col];
                updateInputDisplay();
            }
        }
    }

    if (y > 200) {
        if (x > startX && x < startX + 70) { 
            currentKbdMode = (currentKbdMode == LOWER) ? UPPER : (currentKbdMode == UPPER ? SYMBOLS : LOWER);
            drawKeyboard();
        } 
        else if (x >= startX + 70 && x < startX + 180) { 
            inputBuffer += " "; 
            updateInputDisplay();
        }
        else if (x >= startX + 180) { 
            if (inputBuffer.length() > 0) {
                inputBuffer.remove(inputBuffer.length() - 1);
                updateInputDisplay();
            }
        }
    }
}