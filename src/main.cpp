#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "config.h"
#include "ui.h"
#include "touch.h"
#include "screens.h"
#include "menus/wifi_menu.h"
#include <SD.h>
#include <WiFi.h>

TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen ts(21); 

unsigned long lastInteractionTime = 0;
bool isScreenOff = false;
const unsigned long SLEEP_TIMEOUT = 15000;

void setup() {
    Serial.begin(115200);

    pinMode(TFT_BL, OUTPUT); 
    ledcSetup(0, 5000, 8);
    ledcAttachPin(TFT_BL, 0);
    ledcWrite(0, 150);

    pinMode(5, OUTPUT);  digitalWrite(5, HIGH);
    pinMode(21, OUTPUT); digitalWrite(21, HIGH);
    pinMode(15, OUTPUT); digitalWrite(15, HIGH); 

    delay(200);

    Serial.println(F("Mounting SD..."));
    SPI.begin();
    
    if (!SD.begin(5, SPI, 4000000)) { 
        Serial.println(F("Mounting failed. Retrying at 100kHz..."));
        delay(500);
        if (!SD.begin(5, SPI, 100000)) { 
            Serial.println(F("SD Card Failed permanently!"));
        } else {
            Serial.println(F("SD Card Mounted at 100kHz!"));
        }
    } else {
        Serial.println(F("SD Card Mounted Successfully!"));
    }

    loadSettings();
    ledcWrite(0, currentBrightness);

    tft.init();
    tft.setRotation(1);
    
    ts.begin();
    ts.setRotation(1);

    for(int i = 0; i < 5; i++) {
        if(ts.touched()) ts.getPoint();
        delay(10);
    }

    drawCurrentScreen();
    lastInteractionTime = millis();
}
void loop() {
    static bool lastState = false;
    handleTouch();

    if (!isScreenOff && (millis() - lastInteractionTime > SLEEP_TIMEOUT)) {
        isScreenOff = true;
    }

    if (isScreenOff != lastState) {
        ledcWrite(0, isScreenOff ? 0 : currentBrightness);
        lastState = isScreenOff;
    }

    if (!isScreenOff && currentScreen == WIFI_MENU) {
        static int lastWifiN = -1;
        static wl_status_t lastWifiS = WL_NO_SHIELD;

        int currentN = WiFi.scanComplete();
        wl_status_t currentS = WiFi.status();
        
        if (currentN != lastWifiN || currentS != lastWifiS) {
            lastWifiN = currentN;
            lastWifiS = currentS;
            drawWifiMenu();
        }
    }
}