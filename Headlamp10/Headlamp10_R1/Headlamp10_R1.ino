// port 14120

// Include Libraries
#include "Arduino.h"

String FileName = "Headlamp9_R1";

// ---------------- nRF24L01 ---------------- //
#include <SPI.h>  
#include "RF24.h"
RF24 myRadio (7, 8);  // CE, CSN
const byte address_TR[6] = "10001";  // array of bits Transmitter to Reciever
const byte address_RT[6] = "20001";   

// -------------- data packages ------------- //
// Transmitter to Reciever
struct package_t // create a group of variables called a structure named "package"
  {
    float servo_x; 
    float servo_y; 
    bool joy_sw;
    float brightness;
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


// ------------ 270 servo x left right ------------- //
#include <Servo.h>
#define servo_x_pin 3
Servo servo_x;                                                // servo instance
int x_input;                                              // recieved servo position data

const int x_bound_min = 6;                                    // minimum start position 
const int x_bound_range = 180;                                // range
const int x_bound_max = x_bound_min + x_bound_range;          // maximum end position
const float x_limit_min = 0.0;                           // mechanical min limit of possible positions
const float x_limit_max = 241.0;                        // mechanical max limit, adjusting here sets the spacing ratio correctly
float x_cal;                                              // calibrated x servo position                   

// ms conversion
const float x_ms_max = 2400.0;                       // mechanical ms max
const float x_ms_min = 544.0;                        // mechanical ms min
const float x_ms_per_deg = ((x_ms_max - x_ms_min)/(x_limit_max - x_limit_min));     // ratio between ms to degrees
const float x_ms_b = x_ms_min - (x_ms_per_deg * x_limit_min);                       // y intersept
float x_ms;                                                                         // ms variable


// ----------- 180 servo y up down ------------- //
#define servo_y_pin 2
Servo servo_y;                                    // servo instance
int y_input;                                      // recieved servo position data

const int y_input_min = 0;                        // min input 
const int y_input_max = 180;                      // max input 

const int y_bound_min = 0;                        // minimum digital position
const int y_bound_max = 156;                      // maximum digital position verified
const int y_limit_max = 174;                      // maximum mechanical position before failure
const int y_limit_min = 0;                        // minimum mechanical position before failure
const int y_shift = y_bound_max - y_input_max;    // shifting the angle plane
float y_cal;                                      // calibrated servo position                  


// ----- switch ---- //
bool joy_sw;

// -------------- Lamp ------------- //
#define lamp_pin 5   // mosfet
float brightness = 0;

// ------------------ VOID SETUP -------------- //
void setup() 
{ 
 Serial.begin(115200); delay(200);
 Serial.println(" "); Serial.println(FileName); Serial.println(" ");  
 
 pinMode(lamp_pin, OUTPUT);
 
 servo_y.attach(servo_y_pin);
 servo_x.attach(servo_x_pin);

 // ------------ nRF ------------- //
  boot_radio();  
}


// ------------------- VOID LOOP ------------------ //
void loop() 
{
  myRadio.startListening();
  // -------- read transmission ------- //
  if (myRadio.available())
  {
    while(myRadio.available())
    {
      myRadio.read(&data_t, sizeof(data_t));
      
      brightness = data_t.brightness;
      analogWrite(lamp_pin, brightness);                  // lamp brightness

      joy_sw = data_t.joy_sw;


      // 180 servo y = degrees
      y_input = data_t.servo_y;                                // assign data                      
      y_cal = y_input + y_shift;                               // shifted plane
      y_cal = f_bound(y_cal, y_bound_min, y_bound_max);        // bounds  

      // 270 servo x
      x_input = data_t.servo_x;                                 // assign data
      x_cal = x_input + x_bound_min;                            // start 
      x_cal = f_bound(x_cal, x_bound_min, x_bound_max);         // bound
      x_ms = (x_ms_per_deg * x_cal) + x_ms_b;                   // convert degress to microseond pulses
      
      // write servos
      servo_x.writeMicroseconds(x_ms);                          // Left Right 270 servo calibrated value
      servo_y.write(y_cal);                                     // normal write to motor calibrated value

      Serial.print(" "); Serial.print(FileName); Serial.print(" "); 
      Serial.print("  joySW_ " ); Serial.print(joy_sw); Serial.print(" "); 
      Serial.print("  brightness_ " ); Serial.print(brightness); Serial.print(" ");                                                        
      Serial.print("  x_ " ); Serial.print(x_input); Serial.print(" "); Serial.print(x_cal); Serial.print(" "); Serial.print(x_ms); Serial.print(" ");
      Serial.print("  y_ " ); Serial.print(y_input); Serial.print(" "); Serial.print(y_cal); Serial.print(" ");        
      Serial.println("");
    }
  }
  else
  {
    // Serial.println("No Radio Available, Reboting");      
  }
}

// ------------- Bound ---------------- //
float f_bound(float x, int x_min, int x_max)
{
  if(x > x_max) {x = x_max;}
  if(x < x_min) {x = x_min;}
  return x;
}


void boot_radio()
{
  myRadio.begin(); 
  myRadio.setChannel(115);                     // communication channel
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS);            // maybe the data rate is an issue
  myRadio.openReadingPipe(0, address_TR);
  // myRadio.openWritingPipe(address_RT);
  myRadio.startListening();
}

// Degrees to Ms
float f_DegToMs(int limit_min, int limit_max, int deg)
{
  float ms_min = 544.0;                                           // min ms pulse
  float ms_max = 2400.0;                                          // max ms pulse
  float ms_per_deg = (ms_max - ms_min)/(limit_max - limit_min);   // slope
  float ms_b = ms_min - (ms_per_deg * limit_min);                 // y intercept
  float ms = (ms_per_deg * deg) + ms_b;                           // conversion
  return ms;
}

// specific to motor x
float f_ms_x(int deg)
{
  float ms = f_DegToMs(x_limit_min, x_limit_max, deg);
  return ms;
}

// specific to motor y
float f_ms_y(int deg)
{
  float ms = f_DegToMs(y_limit_min, y_limit_max, deg);
  return ms;
}
