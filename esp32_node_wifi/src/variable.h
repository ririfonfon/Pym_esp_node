#ifndef variable_h
#define variable_h

char ssid[32];
char password[32];

// Default values (Before EEPROM read)

const char *host = "NODE_1";
uint8_t setip4 = 101; // node 1 = 101 , 2 = 102 etc
void init_ssid()
{
    strcpy(ssid, "wskwifi");
    strcpy(password, "WSKwifi69");
}
uint16_t universe_choose = 0;
uint8_t setip1 = 10;

// const char *host = "NODE_1"; // riri dev
// void init_ssid()
// {
// strcpy(ssid, "riri_new");
// strcpy(password, "B2az41opbn6397");
// }
// uint16_t universe_choose = 0;
// uint8_t setip4 = 101;
// uint8_t setip1 = 2;

///////////////////////////////////////////

uint8_t setip2 = 0;
uint8_t setip3 = 0;

int clientn = 0;
#define MAX_CLIENT 6
bool list[MAX_CLIENT] = {false, false, false, false, false, false}; // client
unsigned long screen_off;
bool display_off = false;

#endif