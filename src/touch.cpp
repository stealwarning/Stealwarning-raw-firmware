#include "touch.h"
#include "screens.h"
#include "menus/settings_menu.h"
#include "config.h"

bool isTouched = false;
bool wasTouched = false;

extern unsigned long lastInteractionTime; 
extern bool isScreenOff;

int touchConfidence = 0;

void handleTouch() {
    if (ts.touched()) {
        TS_Point p = ts.getPoint();

        if (p.z < 500) { 
            touchConfidence = 0;
            return;
        }

        lastInteractionTime = millis(); 
        
        if (isScreenOff) {
            isScreenOff = false;
            lastInteractionTime = millis(); 
            wasTouched = true;
            return;
        }

        touchConfidence++;
        if (touchConfidence < 3) return; 

        int x = map(p.x, 240, 3800, 0, 320);
        int y = map(p.y, 240, 3800, 0, 240);

        if (!wasTouched) {
            Serial.printf("Confirmed Touch -> X: %d Y: %d\n", x, y);
            processAllTouches(x, y, false);
            wasTouched = true;
        }
        processAllTouches(x, y, true);
    } else {
        if (wasTouched) {
            wasTouched = false;
            touchConfidence = 0;
            saveSettings();
        }
    }
}