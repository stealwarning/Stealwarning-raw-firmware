#ifndef FILE_EXPLORER_H
#define FILE_EXPLORER_H

#include <Arduino.h>

extern String currentPath;

void drawFileExplorer(const char * dirname);
void handleFileExplorerTouch(int x, int y);
void viewFileContents(const char * path);

#endif