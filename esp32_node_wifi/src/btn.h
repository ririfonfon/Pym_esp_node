#ifndef btn_h
#define btn_h

uint8_t BTN_GPIOPIN = 15;
const int LONG_PRESS_TIME = 1000;
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;
int lastState = LOW;
int currentState = 1;
bool btn = false;
bool start = true;

void init_btn()
{
    pinMode(BTN_GPIOPIN, INPUT);
}

void check_btn()
{
    currentState = digitalRead(BTN_GPIOPIN);

    if (lastState == HIGH && currentState == LOW) // button is pressed
    {
        pressedTime = millis();
    }
    else if (lastState == LOW && currentState == HIGH) // button is released
    {
        releasedTime = millis();

        long pressDuration = releasedTime - pressedTime;

        if (pressDuration > LONG_PRESS_TIME)
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
            Serial.print("(btn_State == HIGH) btn = ");
            Serial.println(btn);
#endif
        }
    }

    // save the the last state
    lastState = currentState;

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