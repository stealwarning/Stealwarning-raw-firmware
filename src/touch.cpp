#include "touch.h"
#include "screens.h"
#include "config.h"

bool wasTouched = false;
int touchConfidence = 0;
extern unsigned long lastInteractionTime;
extern bool isScreenOff;

void handleTouch() {
    if (ts.touched()) {
        TS_Point p = ts.getPoint();
        if (p.z < 500) { touchConfidence = 0; return; }

        lastInteractionTime = millis();
        if (isScreenOff) { isScreenOff = false; wasTouched = true; return; }

        if (++touchConfidence < 3) return; 

        int x = map(p.x, TOUCH_X_MIN, TOUCH_X_MAX, 0, 320);
        int y = map(p.y, TOUCH_Y_MIN, TOUCH_Y_MAX, 0, 240);

        if (!wasTouched) {
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