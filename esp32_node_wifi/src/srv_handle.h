#ifndef srv_handle_h
#define srv_handle_h

#include <EEPROM.h>
#define EEPROM_SIZE 64

void writeStringToEEPROM(int addrOffset, String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}
String readStringFromEEPROM(int addrOffset)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\ 0'; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)
  return String(data);
}

void eeprom_read()
{
    universe_choose = EEPROM.read(1);
    universe_choose |= (EEPROM.read(4) << 24);
    universe_choose |= (EEPROM.read(2) << 8);
    universe_choose |= (EEPROM.read(3) << 16);

    setip1 = EEPROM.read(5);
    setip2 = EEPROM.read(6);
    setip3 = EEPROM.read(7);
    setip4 = EEPROM.read(8);

    String v_ssid = readStringFromEEPROM(9);

    String v_password = readStringFromEEPROM(40);

    *ssid = char(v_ssid.c_str());
    *password = char(v_password.c_str());

#ifdef DEBUG
    Serial.println("EEPROM READ");
    Serial.print(" universe_chosse : ");
    Serial.print(universe_choose);
    Serial.print(" ");
    Serial.print(" set ip : ");
    Serial.print(setip1);
    Serial.print(".");
    Serial.print(setip2);
    Serial.print(".");
    Serial.print(setip3);
    Serial.print(".");
    Serial.print(setip4);
    Serial.println(" ");
    Serial.print(" ssid : ");
    Serial.print(ssid);
    Serial.println(" ");
    Serial.print(" password : ");
    Serial.print(password);
    Serial.println(" ");
#endif
} //eeprom_read

void eeprom_write()
{
    EEPROM.write(1, universe_choose);
    EEPROM.write(2, universe_choose >> 8);
    EEPROM.write(3, universe_choose >> 16);
    EEPROM.write(4, universe_choose >> 24);

    EEPROM.write(5, setip1);
    EEPROM.write(6, setip2);
    EEPROM.write(7, setip3);
    EEPROM.write(8, setip4);

    String ssi = String(ssid);
    String pass = String(password);

    writeStringToEEPROM(9,ssi);

    writeStringToEEPROM(40,pass);

    EEPROM.write(62, 'O');
    EEPROM.write(63, 'K');
    EEPROM.commit();

#ifdef DEBUG
    Serial.println("EEPROM WRITE");
    Serial.print(" universe_chosse : ");
    Serial.print(universe_choose);
    Serial.print(" ");
    Serial.print(" set ip : ");
    Serial.print(setip1);
    Serial.print(".");
    Serial.print(setip2);
    Serial.print(".");
    Serial.print(setip3);
    Serial.print(".");
    Serial.print(setip4);
    Serial.println(" ");
    Serial.print(" ssid : ");
    Serial.print(ssid);
    Serial.println(" ");
    Serial.print(" password : ");
    Serial.print(password);
    Serial.println(" ");
#endif
} //eeprom_write

void load_spec()
{
    eeprom_read();
    for (int i = 0; i < MAX_CLIENT; i++)
    {
        if (list[i])
        {
            webSocket.sendTXT(i, "ba:" + String(lround(universe_choose)));
            webSocket.sendTXT(i, "bb:" + String(lround(setip1)));
            webSocket.sendTXT(i, "bc:" + String(lround(setip2)));

            webSocket.sendTXT(i, "bd:" + String(lround(setip3)));
            webSocket.sendTXT(i, "be:" + String(lround(setip4)));
            webSocket.sendTXT(i, "bf:" + String(ssid));
            webSocket.sendTXT(i, "bg:" + String(password));
        }

    } // for client

} //load_spec

void save_spec()
{
     eeprom_write();
} //save_spec

void init_eeprom()
{
    if (EEPROM.read(62) != 'O' || EEPROM.read(63) != 'K')
    {

#ifdef DEBUG
        Serial.println("failed to initialise EEPROM");
        Serial.println("Formate EEPROM");
#endif

        for (int i = 0; i < EEPROM_SIZE; i++)
        {
            EEPROM.write(i, '\0');
            if (i % 50 == 0)
                delay(100);
        }
#ifdef DEBUG
        Serial.println("EEPROM content cleared!");
#endif

        eeprom_write();

    } //(EEPROM.read(62) != 'O' || EEPROM.read(63) != 'K')

    if (EEPROM.read(62) == 'O' && EEPROM.read(63) == 'K')
    {
        eeprom_read();
    } //(EEPROM.read(62) == 'O' && EEPROM.read(63) == 'K')
} //init_eeprom()

//////////////////////////////////////////srv_handle_set////////////////////////////////////////
void srv_handle_set()
{
    for (uint8_t i = 0; i < server.args(); i++)
    {

        //////////////////////////////////////////setting html///////////////////////////////////////////
        if (server.argName(i) == "mem")
        {
            int LMem = (uint8_t)strtol(server.arg(i).c_str(), NULL, 10);
#ifdef DEBUGSPEC
            Serial.print("mem=");
            Serial.println(LMem);
#endif

            if (LMem == 0)
            {
                load_spec();
            } //if(Mem==0){

            if (LMem == 1)
            {
                save_spec();
            } //if(Mem==1){
        }     //serveur mem

    } //serveur args
    server.send(200, "text/plain", "OK");
} //srv_handle_set()

#endif