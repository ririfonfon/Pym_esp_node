#ifndef screnn_h
#define screen_h

// For a connection via I2C using the Arduino Wire include:
// #include <Wire.h>        // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy: #include "SSD1306.h"

// Initialize the OLED display using Arduino Wire:
// SSD1306Wire display(0x3c, 21, 22); // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
SSD1306Wire display(0x3c, SDA, SCL); // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
// SSD1306Wire display(0x3c, D3, D5);  // ADDRESS, SDA, SCL  -  If not, they can be specified manually.
// SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_32);  // ADDRESS, SDA, SCL, OLEDDISPLAY_GEOMETRY  -  Extra param required for 128x32 displays.
// SH1106Wire display(0x3c, SDA, SCL);     // ADDRESS, SDA, SCL

void init_screen()
{
    // Initialising the UI will init the display too.
    display.init();
    display.flipScreenVertically();
}
void check_display()
{
    if (display_off)
    {
        display.displayOn();
        display_off = false;
        screen_off = millis();
    }
}

void drawProgress(uint8_t progr)
{
    check_display();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 0, "INIT");
    display.drawProgressBar(0, 30, 120, 10, progr);
    display.display();
    delay(500);
}

void drawWifi()
{
    check_display();
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 0, String(ssid));
    display.drawString(64, 20, String(setip1) + String('.') + String(setip2) + String('.') + String(setip3) + String('.') + String(setip4));
    display.drawString(64, 40, "ArtNet: " + String(universe_choose));
    display.display();
    screen_off = millis();
}

void default_display()
{
    check_display();
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 0, "Load");
    display.drawString(64, 20, "To");
    display.drawString(64, 40, "DEFAULT");
    display.display();
}

void default_released_display()
{
    check_display();
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 0, "Released");
    display.drawString(64, 20, "to");
    display.drawString(64, 40, "DEFAULT");
    display.display();
}

void setup_released_display()
{
    check_display();
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 0, "Released");
    display.drawString(64, 20, "to");
    display.drawString(64, 40, "SETUP");
    display.display();
}

void setup_display()
{
    check_display();
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 20, "HTML SETUP");
    display.drawString(64, 40, String(setip1) + String('.') + String(setip2) + String('.') + String(setip3) + String('.') + String(setip4));
    display.display();
}

void off_screen()
{
    display.displayOff();
    display_off = true;
}

#endif