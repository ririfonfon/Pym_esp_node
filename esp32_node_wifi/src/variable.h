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

// const char *host = "NODE_20"; // riri dev
// uint8_t setip4 = 120;
// void init_ssid()
// {
// strcpy(ssid, "riri_new");
// strcpy(password, "B2az41opbn6397");
// }
// uint16_t universe_choose = 8;
// uint8_t setip1 = 2;

///////////////////////////////////////////

uint8_t setip2 = 0;
uint8_t setip3 = 0;

int clientn = 0;
#define MAX_CLIENT 6
bool list[MAX_CLIENT] = {false, false, false, false, false, false}; // client
unsigned long screen_off;
bool display_off = false;

// Default Restore values

uint8_t Rsetip1 = setip1;
uint8_t Rsetip2 = setip2;
uint8_t Rsetip3 = setip3;
uint8_t Rsetip4 = setip4;

uint16_t Runiverse_choose = universe_choose;

#endif