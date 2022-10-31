#ifndef fonction_web_socket_h
#define fonction_web_socket_h

//////////////////////////////////////////websocket///////////////////////////////////////////////
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t lenght)
{
    IPAddress client_ip = webSocket.remoteIP(num);

    switch (type)
    {
    case WStype_DISCONNECTED:
        clientn -= 1;

#ifdef DEBUG
        Serial.print(" nombre de clients : ");
        Serial.print(clientn);
#endif
        break;

    case WStype_CONNECTED:
    {
        clientn += 1;

#ifdef DEBUG
        Serial.print(" nombre de clients : ");
        Serial.print(clientn);
        Serial.printf(" [%u] Connected from %d.%d.%d.%d url: %s\n", num, client_ip[0], client_ip[1], client_ip[2], client_ip[3], payload);
#endif
    }
    break;

    case WStype_TEXT:
    {
        //  is the start for this data

        if (payload[0] == 'b')
        {
#ifdef DEBUGsocket
            Serial.println("payload[0] == 'b'");
#endif
            if (payload[1] == 'a')
            {
                char *pEnd;
                universe_choose = strtol((const char *)&payload[2], &pEnd, 8);
#ifdef DEBUG
                Serial.print(" universe_choose : ");
                Serial.print(universe_choose);
#endif
            } //a
            else if (payload[1] == 'b')
            {
                char *pEnd;
                setip1 = (strtol((const char *)&payload[2], &pEnd, 8));
#ifdef DEBUG
                Serial.print(" setip1 : ");
                Serial.print(setip1);
#endif
            } //b
            else if (payload[1] == 'c')
            {
                char *pEnd;
                setip2 = (strtol((const char *)&payload[2], &pEnd, 8));
#ifdef DEBUG
                Serial.print(" setip2 : ");
                Serial.print(setip2);
#endif
            } //c
            else if (payload[1] == 'd')
            {
                char *pEnd;
                setip3 = (strtol((const char *)&payload[2], &pEnd, 8));
#ifdef DEBUG
                Serial.print(" setip3 : ");
                Serial.print(setip3);
#endif
            } //d
            else if (payload[1] == 'e')
            {
                char *pEnd;
                setip4 = (strtol((const char *)&payload[2], &pEnd, 8));
#ifdef DEBUG
                Serial.print(" setip4 : ");
                Serial.print(setip4);
#endif
            } //e
            else if (payload[1] == 'f')
            {
                char *pEnd;
                // ssid = (&payload[2], &pEnd) ;
#ifdef DEBUG
                Serial.print(" ssid : ");
                Serial.print(ssid);
#endif
            } //f

        } //payload[0] == b

    } //WStype_TEXT:
    break;
    } //type
} //web socket

#endif