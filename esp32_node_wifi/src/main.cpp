
/**************************************************************************/
/*!
    @file     ESP32_NODE_WIFI
    @author   Richard Fontaine
*/
/**************************************************************************/

// platformio run --target uploadfs // cmd console download spiff

#define DEBUG 1

#include <Arduino.h>
#include <variable.h>
#include <EEPROM.h>
#define EEPROM_SIZE 96
#include <dmx.h>
#include <wifi_serv_setup.h>
#include <led.h>
#include <btn.h>

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("setup");
#endif

  EEPROM.begin(EEPROM_SIZE);
  init_eeprom();

  init_led();
  init_dmx();
  ConnectWifi();
  init_artnet();

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
  }

  artnet.read();

  check_btn();
}
