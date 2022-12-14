#ifndef srv_handle_h
#define srv_handle_h

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
    data[newStrLen] = '\0';
    return String(data);
}

void eeprom_read()
{
    universe_choose = EEPROM.read(1);
    universe_choose |= (EEPROM.read(2) << 8);
    universe_choose |= (EEPROM.read(3) << 16);
    universe_choose |= (EEPROM.read(4) << 24);

    setip1 = EEPROM.read(5);
    setip2 = EEPROM.read(6);
    setip3 = EEPROM.read(7);
    setip4 = EEPROM.read(8);

    String v_ssid = readStringFromEEPROM(9);
    String v_password = readStringFromEEPROM(40);

    strcpy(ssid, v_ssid.c_str());
    strcpy(password, v_password.c_str());

#ifdef DEBUG
    Serial.println("EEPROM READ");
    Serial.print(" universe_choose : ");
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
    Serial.print(" ssid : ");
    Serial.print(ssid);
    Serial.println(" ");
    Serial.print(" password : ");
    Serial.print(password);
    Serial.println(" ");
#endif
} // eeprom_read

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

    writeStringToEEPROM(9, ssi);

    writeStringToEEPROM(40, pass);

    EEPROM.write(EEPROM_SIZE - 2, 'O');
    EEPROM.write(EEPROM_SIZE - 1, 'K');
    EEPROM.commit();

#ifdef DEBUG
    Serial.println("EEPROM WRITE");
    Serial.print(" universe_choose : ");
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
} // eeprom_write

void load_spec()
{
    eeprom_read();
    for (int i = 0; i < MAX_CLIENT; i++)
    {
        if (list[i])
        {
#ifdef DEBUG
            Serial.println("load spec send");
#endif
            webSocket.sendTXT(i, "ba:" + String(lround(universe_choose)));
            webSocket.sendTXT(i, "bb:" + String(lround(setip1)));
            webSocket.sendTXT(i, "bc:" + String(lround(setip2)));

            webSocket.sendTXT(i, "bd:" + String(lround(setip3)));
            webSocket.sendTXT(i, "be:" + String(lround(setip4)));
            webSocket.sendTXT(i, "bf:" + String(ssid));
            webSocket.sendTXT(i, "bg:" + String(password));
        }

    } // for client

} // load_spec

void save_spec()
{
    eeprom_write();
    delay(1000);
    ESP.restart();
} // save_spec

void init_eeprom()
{
#ifdef DEBUG
    Serial.printf("eeprom read %d : %c", EEPROM_SIZE - 2, EEPROM.read(EEPROM_SIZE - 2));
    Serial.println("");
    Serial.printf("eeprom read %d : %c", EEPROM_SIZE - 1, EEPROM.read(EEPROM_SIZE - 1));
    Serial.println("");
#endif

    if (EEPROM.read(EEPROM_SIZE - 2) != 'O' || EEPROM.read(EEPROM_SIZE - 1) != 'K')
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
        EEPROM.commit();
#ifdef DEBUG
        Serial.println("EEPROM content cleared!");
#endif

        eeprom_write();

    } //(EEPROM.read(62) != 'O' || EEPROM.read(63) != 'K')

    if (EEPROM.read(EEPROM_SIZE - 2) == 'O' && EEPROM.read(EEPROM_SIZE - 1) == 'K')
    {
#ifdef DEBUG
        Serial.println("read OK OK OK");
#endif
        eeprom_read();
    } //(EEPROM.read(62) == 'O' && EEPROM.read(63) == 'K')
} // init_eeprom()

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
            } // if(Mem==0){

            if (LMem == 1)
            {
                save_spec();
            } // if(Mem==1){
        }     // serveur mem

    } // serveur args
    server.send(200, "text/plain", "OK");
} // srv_handle_set()

#endif