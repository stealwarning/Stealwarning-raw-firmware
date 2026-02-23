#include <SD.h>
#include "file_explorer.h"
#include "ui.h"
#include "theme.h"
#include "draw_icons.h"
#include "screens.h"

String currentPath = "/";
bool isViewingFile = false;

void drawFileExplorer(const char * dirname) {
    isViewingFile = false;
    currentPath = String(dirname);
    tft.fillScreen(currentTheme.background);
    drawStatusBar();
    drawSidebar();

    int startX = 70;
    tft.setTextColor(currentTheme.text);
    tft.setTextSize(2);
    
    tft.setCursor(startX, 35);
    tft.print(F("Dir: "));
    tft.setTextColor(currentTheme.icon);
    tft.print(currentPath);

    File root = SD.open(dirname);
    
    if (!root) {
        tft.setTextColor(0xF800);
        tft.setCursor(startX, 80);
        tft.print(F("Failed to open dir!"));
        return;
    }
    
    if (!root.isDirectory()) {
        tft.setTextColor(0xF800);
        tft.setCursor(startX, 80);
        tft.print(F("Not a directory!"));
        root.close();
        return;
    }

    int yPos = 75;
    int rowHeight = 32;
    int count = 0;

    if (currentPath != "/") {
        tft.drawFastHLine(startX, yPos + rowHeight - 2, 240, currentTheme.statusBar);
        tft.setTextColor(currentTheme.icon);
        tft.setCursor(startX + 30, yPos + 5);
        tft.print(F(".. (Back)"));
        yPos += rowHeight;
        count++;
    }

    File file = root.openNextFile();
    
    if (!file && count == 0) {
        tft.setTextColor(currentTheme.text);
        tft.setCursor(startX, 80);
        tft.print(F("Empty Folder"));
    }

    while (file && count < 6) {
        tft.drawFastHLine(startX, yPos + rowHeight - 2, 240, currentTheme.statusBar);

        if (file.isDirectory()) {
            drawSmallFolder(startX + 5, yPos + 6, currentTheme.icon);
        } else {
            drawSmallFile(startX + 6, yPos + 4, currentTheme.text);
        }

        tft.setTextColor(currentTheme.text);
        tft.setCursor(startX + 30, yPos + 5);
        
        String fname = file.name();
        tft.print(fname.substring(0, 15));

        yPos += rowHeight;
        count++;
        file = root.openNextFile();
    }
    root.close();
}

void viewFileContents(const char * path) {
    isViewingFile = true;
    tft.fillScreen(currentTheme.background);
    drawStatusBar();
    drawSidebar(); 
    
    tft.setTextColor(currentTheme.text);
    tft.setTextSize(1);
    tft.setCursor(70, 35);
    tft.print(F("File: ")); tft.println(path);
    tft.drawFastHLine(70, 48, 240, currentTheme.statusBar);

    File file = SD.open(path);
    if (file) {
        tft.setCursor(70, 55);
        int line = 0;
        while (file.available() && line < 15) {
            tft.println(file.readStringUntil('\n'));
            line++;
        }
        file.close();
    } else {
        tft.setCursor(70, 60);
        tft.setTextColor(0xF800);
        tft.print(F("Error reading file!"));
    }
}

void handleFileExplorerTouch(int x, int y) {
    if (x < 70) return;

    if (isViewingFile) {
        drawFileExplorer(currentPath.c_str());
        return;
    }

    int rowHeight = 32;
    int startY = 75;
    
    if (y < startY) return;
    
    int clickedRow = (y - startY) / rowHeight;
    
    File root = SD.open(currentPath.c_str());
    if (!root) return;

    if (currentPath != "/") {
        if (clickedRow == 0) {
            int lastSlash = currentPath.lastIndexOf('/');
            String parentPath = currentPath.substring(0, lastSlash);
            if (parentPath == "") parentPath = "/";
            drawFileExplorer(parentPath.c_str());
            root.close();
            return;
        }
        clickedRow--;
    }

    int count = 0;
    File file = root.openNextFile();
    while (file) {
        if (count == clickedRow) {
            String newPath = currentPath;
            if (newPath != "/") newPath += "/";
            newPath += file.name();

            if (file.isDirectory()) {
                drawFileExplorer(newPath.c_str());
            } else {
                viewFileContents(newPath.c_str());
            }
            file.close();
            break;
        }
        count++;
        file.close();
        file = root.openNextFile();
    }
    root.close();
}