#ifndef Radio_h
#define Radio_h

// ------------------------------------------ nRF24L01 ------------------------------------ ------- //
RF24 myRadio (7, 8);  // CE, CSN 
const byte addressTR1[6] = "10001";  // Lamp 1 array of bits Transmitter to Reciever
const byte addressTR2[6] = "10002";  // Lamp 2
const byte addressRT1[6] = "20001";  // reciever to transmitter 1
const byte addressRT2[6] = "20002";  // reciever to transmitter 2

// -------------- data packages ------------- //
// Transmitter to Reciever
struct packageT // creates data structure packageT that consists of a group of variables of different types
  {
    float servoX; 
    float servoY; 
    float lampBrightness;
  }; 
  
// typedef struct packageT Package_t; // typedef creates a custom data type "Package" is a differente name for "struct package"
// Package_t data_t;  // make "data" a struct type variable with the format of "Package"

packageT dataT;

// Reciever to Transmitter
struct packageR
  {
    float battVolt; 
    unsigned long recieveTime; 
    int count;
  }; 

packageR dataR;

void radioSetup()
{
  myRadio.begin();  
  myRadio.setChannel(115);              // 125 Possible channels, each channel can have up to 6 addresses. 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS); 
  // myRadio.openReadingPipe(0, address_RT); // Not recieving data
  myRadio.stopListening();
  delay(1000);
}


#endif
