#include "TID.h"

TID::TID()
{
    //create an empty message which can be displayed on the TID
    for (int i=0; i<8; i++) 
    {
		messageTx[i] = ' ';
	}

    SymbolsH = 0;
    SymbolsL = 0;

    //set up the pins which will be used for the communication
    pinMode(SCL, OUTPUT);
    pinMode(MRQ, OUTPUT);
    pinMode(SDA, OUTPUT);
    //activate internal pullups for the communication pins
	digitalWrite(SCL, HIGH);
	digitalWrite(MRQ, HIGH);
	digitalWrite(SDA, HIGH);
}

void TID::startCommunication() 
{
    digitalWrite(MRQ, LOW);
    delayMicroseconds(TID_DELAY * 10);     //250
    digitalWrite(MRQ, HIGH);
    delayMicroseconds(TID_DELAY * 15);     //375
    pinMode(SDA, OUTPUT); //FIXME
    pinMode(SCL, OUTPUT); //FIXME
    digitalWrite(SDA, LOW);
    delayMicroseconds(TID_DELAY * 4);      //100
    digitalWrite(SCL, LOW);
    delayMicroseconds(TID_DELAY * 4);      //100
}

void TID::sendByte(byte data) 
{
    pinMode(SCL, OUTPUT);
    for (byte m = 0X80; m != 0; m >>= 1) 
    {                                             
        digitalWrite(SDA, m & data);              
        delayMicroseconds(TID_DELAY * 12); //300
        digitalWrite(SCL, HIGH);
        delayMicroseconds(TID_DELAY * 2);  //50
        digitalWrite(SCL, LOW);
        delayMicroseconds(TID_DELAY * 2);  //50
    }
    delayMicroseconds(TID_DELAY / 2);      //12,5
    pinMode(SDA, INPUT);
    delayMicroseconds(TID_DELAY / 2);      //12,5
    pinMode(SCL, INPUT);
    delayMicroseconds(TID_DELAY / 2);
    while(digitalRead(SCL) == 0);
    delayMicroseconds(TID_DELAY / 2);      //12,5
    pinMode(SCL,OUTPUT);
    digitalWrite(SCL, LOW);
    while(digitalRead(SDA) == 0);
    pinMode(SDA,OUTPUT);
}

void TID::sendAddress() 
{  
    sendByte(TID_ADDRESS); //slave address of 8 char TID is 0x94 || 10 char TID is 0x9B
    digitalWrite(MRQ, LOW); 
    delayMicroseconds(TID_DELAY / 2);     //12,5
} 

void TID::sendData(byte data) 
{              
    //parity check              
    byte value = data;            
    byte parity;                
    value = value ^ (value >> 4);   
    value = value ^ (value >> 2);   
    value = value ^ (value >> 1);   
    value &= 0x01; //1 if parity is odd, 0 if parity is even
    parity = !value; //add a 1 at LSB position if parity is odd         
    data = (data<<1) | parity;
    sendByte(data); //send byte with even parity to TID
}  

void TID::stopCommunication()
{
    digitalWrite(SDA, LOW);
    delayMicroseconds(TID_DELAY * 20);    //500
    digitalWrite(MRQ, HIGH);
    delayMicroseconds(TID_DELAY * 10);    //250
    digitalWrite(SCL, HIGH);
    delayMicroseconds(TID_DELAY * 4);     //100
    digitalWrite(SDA, HIGH);
    delayMicroseconds(TID_DELAY * 4);     //100
}

void TID::setSymbol(symbols symbol, byte state)
{
	if (symbol == clips) bitWrite(SymbolsH, 0, state);
	if (symbol == as) bitWrite(SymbolsH, 1, state);
	if (symbol == stereo) bitWrite(SymbolsH, 3, state);
	if (symbol == tp) bitWrite(SymbolsH, 4, state);
	if (symbol == rds) bitWrite(SymbolsH, 5, state);
	if (symbol == point) bitWrite(SymbolsH, 6, state);

	if (symbol == cps) bitWrite(SymbolsL, 2, state);
	if (symbol == cr) bitWrite(SymbolsL, 3, state);
	if (symbol == dolbyB) bitWrite(SymbolsL, 4, state);
	if (symbol == dolbyC) bitWrite(SymbolsL, 5, state);
	if (symbol == cd) bitWrite(SymbolsL, 6, state);
}

void TID::setText(String text)
{
    if (text.length() < 8)
    {
        for (unsigned int i = 0; i < text.length(); i++)
        {
            messageTx[i] = text.charAt(i);
        }
    }
    else
    {
        //if text is longer than 8 character it will be cut off 
        for (int j = 0; j < 8; j++)
        {
            messageTx[j] = text.charAt(j);
        }
    }
    
}

void TID::updateTID()
{
	startCommunication();
	sendAddress();
	sendData(SymbolsH);
	sendData(SymbolsL);
	for (int i = 0; i < 8; i++) 
    {
		sendData(messageTx[i]);
	}
	stopCommunication();
}

void TID::clearTID()
{
    for (int i = 0; i < 8; i++) 
    {
		messageTx[i] = ' ';
	}
    SymbolsH = 0;
    SymbolsL = 0;
    updateTID();
}
 
void TID::writeBuffer()
{
      for (int i = 0; i < 8; i++)
        {
            Serial.print(messageTx[i]);
        }
} 