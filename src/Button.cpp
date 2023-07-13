#include "Button.h"

Button::Button()
{
    pinMode(BUTTON_SWITCH, INPUT);
    pinMode(ARDUINO_SELECT, OUTPUT);
    pinMode(RADIO_SELECT, OUTPUT);
    digitalWrite(ARDUINO_SELECT, HIGH); //is radio or arduino shown at the beginning? In this configuration the arduino
    digitalWrite(RADIO_SELECT, LOW);
}

void Button::handleButton()
{
    buttonState = digitalRead(BUTTON_SWITCH);
    if(buttonState == HIGH && buttonPressed == 0) { //button gets first time pressed after last release
        buttonFirstTimePressed = millis();
        buttonPressed = 1;
    } else if(buttonState == LOW && buttonPressed == 1) { //button is not pressed anymore (maybe even before bounce time ended)
        buttonPressed = 0;
    }
    if(buttonState == LOW && buttonShortMode == 1) { //button is not pressed anymore but a short press was registered after debounce
        if(buttonLongMode == 0) { //only register a short button press if there was not a long button press registered yet
            modeChanged = 1;
            displayMode += 1;
            if(displayMode >= 5)
            {
                displayMode = 0;
            }
        } 
        buttonLongMode = 0;
        buttonShortMode = 0;
        buttonTimePressed = 0;
    }

    if(((millis() - buttonFirstTimePressed) >= bounceTime) && buttonPressed == 1) //button is debounced
    {
        buttonShortMode = 1; //short button press is possible
        if(buttonState == HIGH) {
            buttonTimePressed = millis() - buttonFirstTimePressed;
        }

        if(buttonTimePressed >= 1000) { //if button is pressed for more than 1100 ms (1000 ms + debounce time)
            buttonLongMode = 1; //long button press registered -> no short button press possible anymore
            buttonTimePressed = 0;
            buttonPressed = 0; //reset complete button status
            selectDevice += 1;
            if(selectDevice >= 2)
            {
                selectDevice = 0;
            }
            if(digitalRead(ARDUINO_SELECT) == HIGH) //change the multiplexer output (radio <-> arduino)
            {
                digitalWrite(ARDUINO_SELECT, LOW);
                delayMicroseconds(10); //give the switches some time to change the state to avoid shorts and undefined states
                digitalWrite(RADIO_SELECT, HIGH);
            }
            else
            {
                digitalWrite(RADIO_SELECT, LOW);
                delayMicroseconds(10);
                digitalWrite(ARDUINO_SELECT, HIGH);
                
            }
        }
        
    }
}