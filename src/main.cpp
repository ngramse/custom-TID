#include <Arduino.h>
#include <stdint.h>
#include "Pinmapping.h"
#include "TID.h"
#include "Button.h"
#include "Timer.h"
#include "Modes.h"

TID tid;
Button button;
Modes modes;

void setup() {
    // put your setup code here, to run once:
    //Serial.begin(9600);
    initTimerInputCapture();
}

void loop() {
    // put your main code here, to run repeatedly:
    button.handleButton();
    modes.globalCalculations();

    if(button.selectDevice == 1)
    {
        //just calculate but no output because radio output will be displayed
    } 
    else 
    {
        switch (button.displayMode)
        {
            case 0: 
                    if(modes.displayModeName(button.modeChanged) == 1)
                    {
                        tid.setText(">SPEED  ");
                        tid.updateTID();
                    }
                    else
                    {
                        button.modeChanged = 0;
                        tid.setText(modes.getSpeed());
                        tid.updateTID(); 
                    }
                    break;
            case 1: 
                    if(modes.displayModeName(button.modeChanged) == 1)
                    {
                        tid.setText(">AVSPEED");
                        tid.updateTID();
                    }
                    else
                    {
                        button.modeChanged = 0;
                        tid.setText(modes.getAvgSpeed());
                        tid.updateTID(); 
                    }
                    break;
            case 2: 
                    if(modes.displayModeName(button.modeChanged) == 1)
                    {
                        tid.setText(">TRIP   ");
                        tid.updateTID();
                    }
                    else
                    {
                        button.modeChanged = 0;
                        tid.setText(modes.getTrip());
                        tid.updateTID();
                    }
                    break;
            case 3: 
                    if(modes.displayModeName(button.modeChanged) == 1)
                    {
                        tid.setText(">UPTIME ");
                        tid.updateTID();
                    }
                    else
                    {
                        button.modeChanged = 0;
                        tid.setText(modes.getUptime());
                        tid.updateTID();
                    }
                    break;
            case 4: 
                    if(modes.displayModeName(button.modeChanged) == 1)
                    {
                        tid.setText(">VOLTAGE");
                        tid.updateTID();
                    }
                    else
                    {
                        button.modeChanged = 0;
                        tid.setText(modes.getBoardVoltage());
                        tid.updateTID();
                    }
                    break;
            default: tid.setText(" FEHLER ");
                     tid.updateTID();
                     break; 
        }
    }
}