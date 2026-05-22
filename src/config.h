#pragma once

#include <Arduino.h>

// Servo Pins (Configure according to your hardware, e.g., CoreS3 Port B or Port C)
// Note: CoreS3 Port B is G8, G9. Port C is G18, G17
#define SERVO_PAN_PIN 8
#define SERVO_TILT_PIN 9

// Face Colors
#define COLOR_BG M5.Lcd.color565(255, 255, 255) // White
#define COLOR_FG TFT_BLACK

// AP Settings for Config
#define AP_SSID "StackChan_Config"
