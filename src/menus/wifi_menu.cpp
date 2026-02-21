#include <WiFi.h>
#include "wifi_menu.h"
#include "ui.h"
#include "screens.h"
#include "theme.h"

void drawWifiMenu() {
    wl_status_t status = WiFi.status();
    if (status == WL_CONNECTED) selectedSSID = ""; 

    tft.fillScreen(currentTheme.background); 
    drawStatusBar();
    drawNavigationButtons();

    tft.setTextColor(currentTheme.text);
    tft.setTextSize(2);
    tft.setCursor(70, 35);

    if (selectedSSID != "" && status != WL_CONNECTED) {
        tft.print("Connecting...");
        tft.setCursor(70, 65);
        tft.setTextColor(currentTheme.icon);
        tft.print(selectedSSID.substring(0, 15));
        
        tft.setTextColor(currentTheme.text);
        tft.setCursor(70, 100);
        tft.print("Status: Wait...");
        
        tft.drawRoundRect(70, 160, 120, 40, 5, 0xF800); 
        tft.setCursor(85, 172); tft.setTextColor(0xF800); tft.print("CANCEL");
        return; 
    }

    int n = WiFi.scanComplete();
    
    if (status == WL_CONNECTED) {
        tft.print("ON: "); tft.setTextColor(0x07E0);
        tft.print(WiFi.SSID().substring(0, 12));
    } else {
        tft.print("WiFi Networks");
    }

    tft.setTextColor(currentTheme.text);

    if (n == -1 || n == -2) {
        tft.setCursor(70, 80);
        tft.print("Scanning...");
    } else if (n == 0) {
        tft.setCursor(70, 80);
        tft.print("No networks. Scan?");
        WiFi.scanNetworks(true); 
    } else {
        if (n > 5) {
            tft.fillTriangle(295, 75, 285, 95, 305, 95, (wifiOffset > 0) ? currentTheme.icon : currentTheme.statusBar);
            tft.fillTriangle(295, 225, 285, 205, 305, 205, (wifiOffset < n - 5) ? currentTheme.icon : currentTheme.statusBar);
        }

        for (int i = 0; i < 5; i++) {
            int idx = i + wifiOffset;
            if (idx >= n) break;

            int yPos = 75 + (i * 32);
            tft.drawFastHLine(70, yPos + 28, 210, currentTheme.statusBar);
            
            if (status == WL_CONNECTED && WiFi.SSID() == WiFi.SSID(idx)) {
                tft.fillCircle(270, yPos + 12, 6, 0x07E0); 
            } else {
                tft.drawCircle(270, yPos + 12, 6, currentTheme.statusBar);
            }

            tft.setCursor(75, yPos + 5);
            tft.print(WiFi.SSID(idx).substring(0, 12));
        }
    }
}

void handleWifiMenuTouch(int x, int y) {
    if (x < 70) return;

    if (selectedSSID != "" && WiFi.status() != WL_CONNECTED) {
        if (y > 160 && y < 200 && x < 190) {
            WiFi.disconnect();
            selectedSSID = "";
            drawWifiMenu();
        }
        return;
    }

    int n = WiFi.scanComplete();
    
    if (n <= 0) {
        if (y > 70) WiFi.scanNetworks(true);
        return; 
    }

    if (x > 275) {
        if (y < 150 && wifiOffset > 0) { wifiOffset--; drawWifiMenu(); }
        else if (y >= 150 && wifiOffset < n - 5) { wifiOffset++; drawWifiMenu(); }
        return;
    }

    int clickedRow = (y - 75) / 32;
    int idx = clickedRow + wifiOffset;

    if (clickedRow >= 0 && clickedRow < 5 && idx < n) {
        if (WiFi.status() == WL_CONNECTED && WiFi.SSID() == WiFi.SSID(idx)) {
            WiFi.disconnect();
            drawWifiMenu();
        } else {
            selectedSSID = WiFi.SSID(idx);
            currentKbdTarget = KBD_WIFI_PASS;
            changeScreen(KEYBOARD_MENU);
        }
    }
}