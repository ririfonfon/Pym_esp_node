#ifndef variable_h
#define variable_h

// WiFi network settings

const char *host = "esp";
char ssid[32];      
char password[32]; 

int clientn = 0;
#define MAX_CLIENT 6
bool list[MAX_CLIENT] = {false, false, false, false, false, false}; // client

uint8_t setip1 = 2;
uint8_t setip2 = 0;
uint8_t setip3 = 0;
uint8_t setip4 = 115;
IPAddress Ip(2, 0, 0, 112);

uint16_t universe_choose = 1;

unsigned long screen_off;
bool display_off = false; 
#endif