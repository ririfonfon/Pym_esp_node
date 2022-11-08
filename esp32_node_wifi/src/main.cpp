
/**************************************************************************/
/*!
    @file     ESP32_NODE_WIFI
    @author   Richard Fontaine
*/
/**************************************************************************/

// platformio run --target uploadfs // cmd console download spiff

// #define DEBUG 1

#include <Arduino.h>
#include <variable.h>
#include <EEPROM.h>
#define EEPROM_SIZE 96
#include <dmx.h>
#include <wifi_serv_setup.h>
#include <led.h>
#include <screen.h>
#include <btn.h>

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("setup");
#endif

  init_screen();
  drawProgress(10);

  // Default values (Before EEPROM read)
  strcpy(ssid, "riri_new");
  strcpy(password, "B2az41opbn6397");

  // EEPROM read
  EEPROM.begin(EEPROM_SIZE);
  init_eeprom();
  drawProgress(30);

  // INIT
  init_led();
  drawProgress(50);
  init_dmx();
  drawProgress(70);
  ConnectWifi();
  drawProgress(90);
  init_artnet();
  drawProgress(100);

  drawWifi();

#ifdef DEBUG
  Serial.println("setup compete");
#endif
}

void loop()
{
  if (btn == false)
  {
    onboard_led.on = millis() % 2000 < 1000;
    onboard_led.update();

    if (screen_off + 5000 < millis() && display_off == false)
    {
      off_screen();
    }
  }

  artnet.read();

  check_btn();
}
