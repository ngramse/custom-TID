# include "Modes.h"

Modes::Modes()
{
    pinMode(BOARD_VOLTAGE, INPUT);
}

void Modes::globalCalculations() //this values must be calculated all the time no matter in which mode the display is
{
    prevUptime = uptime;
    uptime = (millis() / 1000);
    prevMeters = meters;
    meters = (meters + (odometerCount * METERS_PER_IMPULSE)) + 0.5; //round up the result with 0.5
    odometerCount = 0;
}

unsigned int Modes::displayModeName(unsigned int changeMode)
{
    if(changeMode == 0) //display mode has not changed -> nothing is to do 
    {
        lastButtonTime = uptime;
        return 0;
    }
    else //display mode has changed -> the modes name will be displayed for a few seconds
    {
        if(uptime - lastButtonTime <= 3)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

String Modes::getSpeed()
{
    String speedString = "";
    speed = (((prevImpulseCount * METERS_PER_IMPULSE) / IMPULSE_MEASURING_SECONDS) * 3.6) + 0.8; //correction factor of 0.8
    if(speed <= 9)
    {
        speedString = space + space + speed + unitKmh; //"  x KM/H"
    }
    else if(speed <= 99)
    {
        speedString = space + speed + unitKmh; //" xx KM/H"
    }
    else
    {
        speedString = speed + unitKmh; //"xxx KM/H"
    }
    return speedString;
}

String Modes::getAvgSpeed()
{
    String avgSpeedString = "";
    avgSpeed = ((meters / uptime) * 3.6) + 0.5; //round up the result with 0.5
    if(avgSpeed <= 9)
    {
        avgSpeedString = space + space + avgSpeed + unitKmh; //"  x KM/H"
    }
    else if(avgSpeed <= 99)
    {
        avgSpeedString = space + avgSpeed + unitKmh; //" xx KM/H"
    }
    else
    {
        avgSpeedString = avgSpeed + unitKmh; //"xxx KM/H"
    }
    return avgSpeedString;
}

String Modes::getTrip()
{
    String tripString = "";
    float tripKilometers = (meters / 1000.0);
    tripString = String(tripKilometers, 1);
    tripString.replace('.', ',');

    if(tripKilometers <= 9)
    {
        tripString = space + space + tripString + unitKm; //"  x.x KM"
    }
    else if(tripKilometers <= 99)
    {
        tripString = space + tripString + unitKm; //" xx.x KM"
    }
    else
    {
        tripString = tripString + unitKm; //"xxx.x KM"
    }

    return tripString;
}

String Modes::getUptime()
{
    String uptimeString = "";
    unsigned int seconds = uptime % 60;
    unsigned int minutes = (uptime / 60) % 60;
    unsigned int hours = (uptime / 3600);
    if (seconds <= 9)
    {
        if (minutes <= 9)
        {
            if (hours <= 9)
            {
                uptimeString = leadingZero + hours + clockSeparator + leadingZero + minutes + clockSeparator + leadingZero + seconds;
            }
            else
            {
                uptimeString = hours + clockSeparator + leadingZero + minutes + clockSeparator + leadingZero + seconds;
            }
        }
        else
        {
            if (hours <= 9)
            {
                uptimeString = leadingZero + hours + clockSeparator + minutes + clockSeparator + leadingZero + seconds;
            }
            else
            {
                uptimeString = hours + clockSeparator + minutes + clockSeparator + leadingZero + seconds;
            }
        }
    }
    else
    {
        if (minutes <= 9)
        {
            if (hours <= 9)
            {
                uptimeString = leadingZero + hours + clockSeparator + leadingZero + minutes + clockSeparator + seconds;
            }
            else
            {
                uptimeString = hours + clockSeparator + leadingZero + minutes + clockSeparator + seconds;
            }
        }
        else
        {
            if (hours <= 9)
            {
                uptimeString = leadingZero + hours + clockSeparator + minutes + clockSeparator + seconds;
            }
            else
            {
                uptimeString = hours + clockSeparator + minutes + clockSeparator + seconds;
            }
        }
    }
    return uptimeString;
}

String Modes::getBoardVoltage()
{
    String voltageString = "";
    float carVoltage = 0.0;

    boardVoltage = analogRead(BOARD_VOLTAGE);

    carVoltage = (((float)boardVoltage / 1023.0) * 5.16) + 0.3; //very rough calculation its just to see for example if the alternator works correct
    carVoltage = ((R1 + R2) / R2) * carVoltage;
    carVoltage = carVoltage + 0.005;

    if (carVoltage >= 10.0)
    {
        voltageString = String(carVoltage, 2);
        voltageString.replace('.', ',');
        voltageString = voltageString + unitV + space;
        return voltageString;
    }
    else
    {
        voltageString = String(carVoltage, 2);
        voltageString.replace('.', ',');
        voltageString = voltageString + unitV + space + space;
        return voltageString;
    }
}