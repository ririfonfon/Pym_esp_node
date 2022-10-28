
/**************************************************************************/
/*!
    @file     ESP32_NODE_WIFI
    @author   Richard Fontaine
*/
/**************************************************************************/

#include <ArtnetWifi.h>
#include <Arduino.h>
#include <LXESP32DMX.h>
#include "esp_task_wdt.h"

struct Led
{
  // state variables
  uint8_t pin;
  bool on;

  // methods
  void update()
  {
    digitalWrite(pin, on ? HIGH : LOW);
  }
};
Led onboard_led = {2, false};

// Artnet settings

uint16_t universe_choose = 0;

// Wifi settings
// const char *ssid = "ssid";
// const char *password = "pAsSwOrD";

const char *ssid = "riri_new";
const char *password = "B2az41opbn6397";

WiFiUDP UdpSend;
ArtnetWifi artnet;

// connect to wifi – returns true if successful or false if not
bool ConnectWifi(void)
{
  bool state = true;
  int i = 0;

  IPAddress Ip(2, 0, 0, 112);
  IPAddress NMask(255, 0, 0, 0);
  WiFi.config(Ip, Ip, NMask);

  WiFi.begin(ssid, password);
  ESP_LOGD("", "");
  ESP_LOGD("", "Connecting to WiFi");

  // Wait for connection
  ESP_LOGD("", "Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    ESP_LOGD("", ".");
    if (i > 20)
    {
      state = false;
      break;
    }
    i++;
  }
  if (state)
  {
    ESP_LOGD("", "");
    ESP_LOGD("", "Connected to : %s", ssid);
    ESP_LOGD("","STA IP: %u.%u.%u.%u",
            Ip[0], Ip[1], Ip[2], Ip[3]);
  }
  else
  {
    ESP_LOGD("", "");
    ESP_LOGD("", "Connection failed.");
  }

  return state;
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data)
{
    ESP_LOGD("", "DMX: Univ: %x , Seq: %x , Data (%x): ", universe, sequence, length);
  if (universe == universe_choose)
  {
    // send out the buffer
    xSemaphoreTake(ESP32DMX.lxDataLock, portMAX_DELAY);
    for (int i = 0; i < 511; i++)
    {
      ESP32DMX.setSlot(i + 1, data[i]);
    }
    xSemaphoreGive(ESP32DMX.lxDataLock);
  }
}

#define DMX_DIRECTION_PIN 4
#define DMX_SERIAL_OUTPUT_PIN 16

void setup()
{
  Serial.begin(115200);

  pinMode(onboard_led.pin, OUTPUT);

  ESP_LOGD("", "setup");

  pinMode(DMX_DIRECTION_PIN, OUTPUT);
  digitalWrite(DMX_DIRECTION_PIN, HIGH);

  pinMode(DMX_SERIAL_OUTPUT_PIN, OUTPUT);
  ESP32DMX.startOutput(DMX_SERIAL_OUTPUT_PIN);
  ESP_LOGD("", "setup complete");

  ConnectWifi();

  // this will be called for each packet received
  artnet.setArtDmxCallback(onDmxFrame);
  artnet.begin();
}

void loop()
{
  onboard_led.on = millis() % 2000 < 1000;
  onboard_led.update();
  // we call the read function inside the loop
  artnet.read();
}
