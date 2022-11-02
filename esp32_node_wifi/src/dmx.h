#ifndef dmx_h
#define dmx_h

#include <LXESP32DMX.h>
#include "esp_task_wdt.h"

#define DMX_DIRECTION_PIN 4
#define DMX_SERIAL_OUTPUT_PIN 16

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data)
{
  // ESP_LOGD("", "DMX: Univ: %x , Seq: %x , Data (%x): ", universe, sequence, length);
  if (universe == universe_choose)
  {
    // send out the buffer
    xSemaphoreTake(ESP32DMX.lxDataLock, portMAX_DELAY);
    for (int i = 0; i < length; i++)
    {
      ESP32DMX.setSlot(i + 1, data[i]);
    }
    xSemaphoreGive(ESP32DMX.lxDataLock);
  }
}

void init_dmx()
{
  pinMode(DMX_DIRECTION_PIN, OUTPUT);
  digitalWrite(DMX_DIRECTION_PIN, HIGH);

  pinMode(DMX_SERIAL_OUTPUT_PIN, OUTPUT);
  ESP32DMX.startOutput(DMX_SERIAL_OUTPUT_PIN);
}

#endif