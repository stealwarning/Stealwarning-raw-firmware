# Stealwarning raw firmware for ESP32 + ILI9341 touch display

## What forks?
1. Theme selector
2. Touch
3. Brightness
4. Keyboard
5. Memory
6. Sleep (save screen)
7. File Explorer
8. Wifi connect With bugs

# Display Pin Configuration

| Display Pin | ESP32 Pin | Function | Notes |
|-------------|-----------|----------|-------|
| VCC | 3.3V | Power Supply | (Or 5V if voltage regulator is present) |
| GND | GND | Ground | |
| CS (TFT) | GPIO 15 | TFT Chip Select | |
| RESET | GPIO 4 | Reset | |
| DC / RS | GPIO 22 | Data / Command | Don't use GPIO 2! |
| SDI (MOSI) | GPIO 23 | SPI MOSI | Shared for TFT, Touch, SD |
| SCK | GPIO 18 | SPI Clock | Shared for TFT, Touch, SD |
| LED | GPIO 32 | Backlight (BL) | Don't use GPIO 0! |
| SDO (MISO) | GPIO 19 | SPI MISO | Shared for TFT, Touch, SD |
| T_CLK | GPIO 18 | SPI Clock | Connect to SCK |
| T_CS | GPIO 21 | Touch Chip Select | |
| T_DIN | GPIO 23 | SPI MOSI | Connect to SDI |
| T_DO | GPIO 19 | SPI MISO | Connect to SDO |
| SD_CS | GPIO 5 | SD Chip Select | |

## Important Notes

- **Avoid GPIO 2** for DC/RS connection
- **Avoid GPIO 0** for LED backlight
- SPI pins (MOSI, MISO, SCK) are shared between TFT display, touch controller, and SD card
- Touch controller pins should be connected to the corresponding SPI pins as indicated
