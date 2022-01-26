#ifndef Radio_h
#define Radio_h

#include "Gimbal.h"
#include "Lamp.h"


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
    int lampBrightness;
    bool joySW;
  }; 
  
typedef struct packageT PackageT; // typedef creates a custom data type "Package" is a differente name for "struct package"
// Package_t data_t;  // make "data" a struct type variable with the format of "Package"
PackageT dataT;

// Reciever to Transmitter
struct packageR
  {
    float battVolt; 
    unsigned long recieveTime; 
    int count;
  }; 

packageR dataR;


// modify data to prepare to send
// @param degs[0] (xDeg lamp 1)
// @param degs[1] (yDeg lamp 1)
// @param mirrorState (mirror lamps or not)
// @return degs[2] (xDeg lamp 2)
// @return degs[3] (yDeg lamp 2)
void dataPrep()
{
  lampBrightness = normalizeLamp(lampBrightness);     // converts [0-100] to [0-255]
  if(mirrorState == true) {degs[2] = 180 - degs[0];}  // X Lamps Mirror
  else {degs[2] = degs[0];}                           // X Lamps Follow
  degs[3] = degs[1];                                  // Y Lamps Follow

  // Bound servo degree
  for(int i=0; i<sizeDegs; i++)
  {
    degs[i] = bound(degs[i], degMin, degMax);
  }
}


// assign data into proper transmission packet
// @param lampBrightness
// @param joySW
// @param degs[] (all servo degrees)
// addressTR1 (address transmitter to reciever 1)
// addressTR2 (address trasmitter to reciever 2)
void dataTransmit()
{
  // ---------- populate & transmit data packet ------- //
  // transmit to both        
  dataT.lampBrightness = lampBrightness;
  dataT.joySW = joySW;

  // transmit to lamp 1
  dataT.servoX = degs[0];   // servo_x1_degree
  dataT.servoY = degs[1];   // servo__y1_degree
  myRadio.openWritingPipe(addressTR1);   // Lamp 1
  myRadio.write(&dataT, sizeof(dataT));

  // transmit to lamp 2
  dataT.servoX = degs[2];   // servo_x2_degree
  dataT.servoY = degs[3];   // servo_y2_degree
  myRadio.openWritingPipe(addressTR2);   // Lamp 2
  myRadio.write(&dataT, sizeof(dataT));
}


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
