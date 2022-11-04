#ifndef btn_h
#define btn_h

uint8_t BTN_GPIOPIN = 15;
const int LONG_PRESS_TIME = 1000;
const int RESET_PRESS_TIME = 10000;
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;
int lastState = LOW;
int currentState;
bool btn = false;
bool start = true;

void init_btn()
{
    pinMode(BTN_GPIOPIN, INPUT);
}

void check_btn()
{
    currentState = digitalRead(BTN_GPIOPIN);

    if (lastState == LOW && currentState == HIGH) // button is pressed
    {
        pressedTime = millis();
        lastState = currentState;
    }
    else if (lastState == HIGH && currentState == LOW) // button is released
    {
        releasedTime = millis();

        long pressDuration = releasedTime - pressedTime;

        if (pressDuration > RESET_PRESS_TIME)
        {
            universe_choose = 3;
            setip1 = 2;
            setip2 = 0;
            setip3 = 0;
            setip4 = 110;
            strcpy(ssid, "riri_new");
            strcpy(password, "B2az41opbn6397");
            eeprom_write();

#ifdef DEBUG
            Serial.print("RESET_PRESS btn = ");
            Serial.println(btn);
#endif
        }
        else if (pressDuration > LONG_PRESS_TIME)
        {

            if (btn == true)
            {
                btn = false;
            }
            else if (btn == false)
            {
                btn = true;
                start = true;
                close_serv();
            }
#ifdef DEBUG
            Serial.print("LONG_PRESS btn = ");
            Serial.println(btn);
#endif
        }
        lastState = currentState;
    }

    if (btn)
    {
        if (start)
        {
            start = false;
            init_serv();
        }
        webSocket.loop();
        server.handleClient();
        onboard_led.on = millis() % 400 < 200;
        onboard_led.update();
    }
}

#endif