/*
 * Button.h - Controls the connected button of the arduino 
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "Pinmapping.h"

class Button 
{
public:
    Button();
    unsigned int buttonState = 0;
    unsigned int displayMode = 0; //0 = speed, 1 = avgSpeed, 2 = trip, 3 = uptime, 4 = boardVoltage
    unsigned int selectDevice = 0; //0 = radio, 1 = arduino
    unsigned int modeChanged = 0; //1 = mode changed
    void handleButton();

private:
    unsigned int bounceTime = 100; //milliseconds
    unsigned int buttonPressed = 0;
    unsigned int buttonShortMode = 0;
    unsigned int buttonLongMode = 0;
    unsigned long buttonFirstTimePressed = 0;
    unsigned long buttonTimePressed = 0;
};

#endif