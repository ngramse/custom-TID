/*
 * TID.h - Description of the TID protocol
 */

#ifndef TID_H
#define TID_H

#include <Arduino.h>
#include "Pinmapping.h"

#define TID_DELAY 25
#define TID_ADDRESS 0x94

class TID 
{
public:
    enum symbols {dolbyC, dolbyB, as, cr, stereo, cd, rds, cps, clips, tp, point};

    TID();
    void setSymbol(symbols symbol, byte state);
    void setText(String text);
    void updateTID();
    void clearTID();
    void writeBuffer();

private:
    char messageTx[9];
    byte SymbolsH;
    byte SymbolsL;

    void startCommunication();
    void sendByte(byte data);
    void sendAddress();
    void sendData(byte data);
    void stopCommunication();
};

#endif