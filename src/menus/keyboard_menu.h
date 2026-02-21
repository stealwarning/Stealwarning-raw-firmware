#ifndef KEYBOARD_MENU_H
#define KEYBOARD_MENU_H

#include <Arduino.h>

void drawKeyboard();
void handleKeyboardMenuTouch(int x, int y);
extern String inputBuffer;

#endif