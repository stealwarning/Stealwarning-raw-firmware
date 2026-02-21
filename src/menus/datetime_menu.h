#ifndef DATETIME_MENU_H
#define DATETIME_MENU_H

#include <TFT_eSPI.h>
#include "screens.h"
#include "menus/main_menu.h"
#include "menus/settings_menu.h"
#include "menus/keyboard_menu.h"
#include "menus/settings_menu.h"
#include "theme.h"
#include "touch.h"
#include "config.h"
#include "ui.h"

#include <Arduino.h>

void drawDateTimeMenu();
void handleDateTimeTouch(int x, int y); 

#endif