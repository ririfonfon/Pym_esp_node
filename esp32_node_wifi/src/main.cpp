
/**************************************************************************/
/*!
    @file     ESP32_NODE_WIFI
    @author   Richard Fontaine
*/
/**************************************************************************/

#include <ArtnetWifi.h>
#include <Arduino.h>

#include <WiFi.h>
#include <WiFiAP.h>

#include <WebSocketsServer.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>

#include <EEPROM.h>
#define EEPROM_SIZE 64

#include <LXESP32DMX.h>
#include "esp_task_wdt.h"


int clientn = 0;
#define MAX_CLIENT 6
bool list[MAX_CLIENT] = {false, false, false, false, false, false}; // client

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Artnet settings
uint16_t universe_choose = 0;

// Wifi settings
char *ssid = "riri_new";
char *password = "B2az41opbn6397";
uint8_t setip1,setip2,setip3,setip4;

#include <fonction_web_socket.h>

const char *host = "esp";

#include "srv_handle.h"
#include <fonction_spiffs.h>

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
    ESP_LOGD("", "STA IP: %u.%u.%u.%u",
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

  //////////////////////////////////////////////////// SPIFFS
  SPIFFS.begin();
  listDir(SPIFFS, "/", 0);

  //////////////////////////////////////////////////// SERVER INIT
  // list directory
  server.on("/list", HTTP_GET, handleFileList);
  // load editor
  server.on("/edit", HTTP_GET, []()
            {
    if (!handleFileRead("/edit.html"))
      server.send(404, "text/plain", "FileNotFound"); });
  // create file
  server.on("/edit", HTTP_PUT, handleFileCreate);
  // delete file
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  // first callback is called after the request has ended with all parsed arguments
  // second callback handles file uploads at that location
  server.on(
      "/edit", HTTP_POST, []()
      { server.send(200, "text/plain", ""); },
      handleFileUpload);

  // called when the url is not defined here
  // use it to load content from SPIFFS
  server.onNotFound([]()
                    {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound"); }); // server.onNotFound

  // start webSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  webSocket.enableHeartbeat(15000, 3000, 2);

  if (MDNS.begin(host))
  {
#ifdef DEBUG
    Serial.println("MDNS responder started");
#endif
  }

  // handle index
#ifdef DEBUG
  Serial.println("HTTP server setup");
#endif

  // Serveur
  server.on("/set", srv_handle_set);
  server.serveStatic("/", SPIFFS, "/pym.html");
  server.serveStatic("/pym.js", SPIFFS, "/pym.js");
  server.serveStatic("/pym.css", SPIFFS, "/pym.css");
  server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
  server.begin();

  MDNS.addService("http", "tcp", 80);
  MDNS.addService("ws", "tcp", 81);

  //////////////////////////////////////////////////// EEPROM
  init_eeprom();
  eeprom_read();

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}

void loop()
{
  onboard_led.on = millis() % 2000 < 1000;
  onboard_led.update();
  // we call the read function inside the loop
  artnet.read();
}
