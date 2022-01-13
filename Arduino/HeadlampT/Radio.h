#ifndef Radio_h
#define Radio_h

// ------------------------------------------ nRF24L01 ------------------------------------ ------- //
RF24 myRadio (7, 8);  // CE, CSN 
const byte address1_TR[6] = "10001";  // Lamp 1 array of bits Transmitter to Reciever
const byte address2_TR[6] = "10002";  // Lamp 2
const byte address_RT[6] = "20001";   

// -------------- data packages ------------- //
// Transmitter to Reciever
struct package_t // create a group of variables called a structure named "package"
  {
    float servoX; 
    float servoY; 
    bool joySW;
    float lampBrightness;
  }; 
  
typedef struct package_t Package_t; // typedef creates a custom data type "Package" is a differente name for "struct package"
Package_t data_t;  // make "data" a struct type variable with the format of "Package"

// Reciever to Transmitter
struct package_r
  {
    float volt_batt; 
    unsigned long recieveTime; 
    int count;
  }; 
typedef struct package_r Package_r;
Package_r data_r;


void radioSetup()
{
  myRadio.begin();  
  myRadio.setChannel(115);              // 125 Possible channels, each channel can have up to 6 addresses. 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS ); 
  // myRadio.openReadingPipe(0, address_RT); // Not recieving data
  myRadio.stopListening();
  delay(1000);
}


#endif
