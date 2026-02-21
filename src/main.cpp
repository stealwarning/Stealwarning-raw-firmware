#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "config.h"
#include "ui.h"
#include "touch.h"
#include "screens.h"
#include "menus/wifi_menu.h"
#include <Preferences.h>
#include <SD.h>
#include <WiFi.h>

TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen ts(21); 

unsigned long lastInteractionTime = 0;
bool isScreenOff = false;
const unsigned long SLEEP_TIMEOUT = 15000;

void setup() {
    Serial.begin(115200);

    pinMode(5, OUTPUT);  digitalWrite(5, HIGH);  
    pinMode(21, OUTPUT); digitalWrite(21, HIGH); 
    pinMode(15, OUTPUT); digitalWrite(15, HIGH); 

    delay(200);

    Serial.println("Mounting SD...");
    if (!SD.begin(5, SPI, 4000000)) { 
        Serial.println("Mounting failed. Retrying at lower speed...");
        delay(500);
        if (!SD.begin(5, SPI, 400000)) { 
            Serial.println("SD Card Failed permanently!");
        } else {
            Serial.println("SD Card Mounted at 400kHz!");
        }
    } else {
        Serial.println("SD Card Mounted Successfully!");
    }

    loadSettings();

    tft.init();
    tft.setRotation(1);
    
    ts.begin();
    ts.setRotation(1);

    for(int i = 0; i < 5; i++) {
        if(ts.touched()) ts.getPoint();
        delay(10);
    }

    ledcSetup(0, 5000, 8);
    ledcAttachPin(TFT_BL, 0);
    ledcWrite(0, currentBrightness);

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
        if (isScreenOff) {
            ledcWrite(0, 0);
            Serial.println("Hardware: Display Sleep");
        } else {
            ledcWrite(0, currentBrightness);
            Serial.println("Hardware: Display Wake");
        }
        lastState = isScreenOff;
    }

    static int lastWifiN = -1;
    static wl_status_t lastWifiS = WL_NO_SHIELD;

    if (currentScreen == WIFI_MENU) {
        int currentN = WiFi.scanComplete();
        wl_status_t currentS = WiFi.status();
        
        if (currentN != lastWifiN || currentS != lastWifiS) {
            lastWifiN = currentN;
            lastWifiS = currentS;
            drawWifiMenu();
        }
    }
}