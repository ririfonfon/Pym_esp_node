#ifndef wifi_serv_setup_h
#define wifi_serv_setup_h

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <ArtnetWifi.h>
#include <WebSocketsServer.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

WiFiUDP UdpSend;

// Artnet settings
ArtnetWifi artnet;

#include "fonction_spiffs.h"
#include "fonction_web_socket.h"
#include "srv_handle.h"

// connect to wifi – returns true if successful or false if not
bool ConnectWifi(void)
{
    bool state = true;
    int i = 0;

    //////////////////////////////////////////////////// EEPROM
    eeprom_read();

    IPAddress Ip(setip1, setip2, setip3, setip4);
    IPAddress NMask(255, 0, 0, 0);
    WiFi.config(Ip, Ip, NMask);
    WiFi.setHostname(host);

    WiFi.begin(ssid, password);
#ifdef DEBUG
    Serial.println("Connecting to WiFi");
#endif

// Wait for connection
#ifdef DEBUG
    Serial.println("Connecting");
#endif
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
#ifdef DEBUG
        Serial.print(".");
#endif
        if (i > 20)
        {
            state = false;
            break;
        }
        i++;
    }
    if (state)
    {
#ifdef DEBUG
        Serial.println("");
        Serial.printf("Connected to : %s", ssid);
        Serial.println("");
        Serial.printf("STA IP: %u.%u.%u.%u",
                      Ip[0], Ip[1], Ip[2], Ip[3]);
        Serial.println("");
#endif
        WiFi.setAutoReconnect(true);
        WiFi.persistent(true);
    }
    else
    {
#ifdef DEBUG
        Serial.println("Connection failed.");
#endif
    }

    return state;
}

//////////////////////////////////////////serveur////////////////////////////////////////
void close_serv()
{
    SPIFFS.end();
    server.close();
    webSocket.close();
    MDNS.end();
}
void init_serv()
{
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

} // init_wifi

void init_artnet()
{
    // this will be called for each packet received
    artnet.setArtDmxCallback(onDmxFrame);
    artnet.begin();
} // init_artnet
#endif