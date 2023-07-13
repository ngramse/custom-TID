/*
 * Modes.h - Functions of the different Modes that can be displayed on the TID
 */

#ifndef MODES_H
#define MODES_H

#include <Arduino.h>
#include "Pinmapping.h"
#include "Timer.h"

#define METERS_PER_IMPULSE 0.059 //U(tires) / Impulses
#define IMPULSE_MEASURING_SECONDS 0.2125 // t = (3.6 * U(tires) / 29(impulses per wheel rotation))
#define R1 10000
#define R2 4700

class Modes
{
public:
    Modes();
    void globalCalculations();
    unsigned int displayModeName(unsigned int changeMode);
    String getSpeed();
    String getAvgSpeed();
    String getTrip();
    String getUptime();
    String getBoardVoltage();
    
private:
    unsigned int speed = 0;
    unsigned int avgSpeed = 0;
    unsigned long meters = 0; 
    unsigned long prevMeters = 0;
    unsigned long uptime = 0;
    unsigned long prevUptime = 0;
    unsigned long lastButtonTime = 0;
    int boardVoltage = 0;

    String unitKmh = " KM/H";
    String unitKm = " KM";
    String unitV = " V";
    String space = " ";
    String separator = ",";
    String clockSeparator = "-";
    String leadingZero = "0";
};

#endif