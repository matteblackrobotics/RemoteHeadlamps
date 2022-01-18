#ifndef Radio_h
#define Radio_h

// ---------------- nRF24L01 ---------------- //
#include <SPI.h>  
#include "RF24.h"
RF24 myRadio (7, 8);  // CE, CSN
const byte addressTR[6] = "10001";  // array of bits Transmitter to Reciever
const byte addressRT[6] = "20001";   

// -------------- data packages ------------- //
// Transmitter to Reciever
struct packageT // create a group of variables called a structure named "package"
  {
    float servoX; 
    float servoY; 
    int lampBrightness;
    bool joySW;
  }; 
  
typedef struct packageT PackageT; // typedef creates a custom data type "Package" is a differente name for "struct package"
PackageT dataT;  // make "data" a struct type variable with the format of "Package"

// Reciever to Transmitter
struct packageR
  {
    float battVolt; 
    unsigned long recieveTime; 
    int count;
  }; 
typedef struct packageR PackageR;
PackageR dataR;


void setupRadio()
{
  myRadio.begin(); 
  myRadio.setChannel(115);                     // communication channel
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS);            // maybe the data rate is an issue
  myRadio.openReadingPipe(0, addressTR);
  // myRadio.openWritingPipe(addressRT);
  myRadio.startListening();
}



#endif