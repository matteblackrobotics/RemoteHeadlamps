

// port 14110

// Use classes and objects to get data from joystick
// 180 servos i guess
// https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
// https://create.arduino.cc/projecthub/MissionCritical/mpu-6050-tutorial-how-to-program-mpu-6050-with-arduino-aee39a

// board voltages
// float volts_5 = 5000.0;
float volts_12 = 12000.0;
float volts_3_3 = 3300.0; 
int print_mode;

// Include Libraries
#include "Arduino.h"
#include <myFunctions2.h>

String FileName = "Headlamp9_T";


// ----------------------------------------- Joystick --------------------------------------- //
#include "Joystick.h"
#define joy_x_pin A6
#define joy_y_pin A7
#define joy_sw_pin 6
Joystick joy1(joy_x_pin, joy_y_pin, joy_sw_pin); // initialize - class: Joystick - object: joy1
float joy1_x;
float joy1_y;
bool joy1_sw;
bool joy1_sw_last;
int joy_max = 1;
int joy_min = -1;


// -------------- lamp ---------------- //
float brightness;

// --------------- Pot ---------------- //
#define pot_pin A3
int pot_cal;            // inital position of pot
int pot_val;            // position of steering pot
int pot_deg;
float pot_volt;
const int pot_min = 0;
const int pot_max = 800;
const int pot_mid = 400;
const int pot_Vin = 5;        //?????

float pot_rot;    // rotations of pot
const int pot_rot_max = 10;
const int pot_rot_mid = 5;
int pot_deg_max = pot_rot_max * 360;  // 3600

int lamp_pos; // relative lamp position from forward

// ------------- wheel --------------- //
const float wheel_rot_max = 2.5;
float wheel_rot;


// ------------------------------------------ nRF24L01 ------------------------------------ ------- //
#include <SPI.h>  
#include "RF24.h"
RF24 myRadio (7, 8);  // CE, CSN 
const byte address1_TR[6] = "10001";  // Lamp 1 array of bits Transmitter to Reciever
const byte address2_TR[6] = "10002";  // Lamp 2
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


// ------------------------------------------- Servos ------------------------------------------ //
int servo_x_deg;                // individual servo position
int servo_y_deg; 

int deg_max = 180;    // min bound for servo position due to power issues??    // desired max
int deg_min = 0;    // min bound for servo position due to power issues??     // desired min
int deg_mid = 180/2;  // correct mid position for servo

int degs[4] = {deg_min, deg_max, deg_min, deg_max};      // servo degree array {x1, y1, x2, y2} initial position
int size_degs = sizeof(degs)/sizeof(degs[0]);           // size of degree array
int mech0s[4] = {0, 0, 0, 0};                           // mech 0 (deg)
int steps[2] = {0, 0};                                   // step x, y

int target_x; 
int target_y;
int step_x; 
int step_y;
int step_max = 5;
int step_min = 1; 
int step_thresh1 = 10;

// ------------------------------------------ LED --------------------------------------------- //
#include <Adafruit_NeoPixel.h>
#define led_pin 2
#define num_leds 1
int LED_index = 0;
int R; // [0:255]
int G;
int B;
Adafruit_NeoPixel LEDstrip(num_leds, led_pin, NEO_GRB + NEO_KHZ800);

// -------------------------------------- button press --------------------------------- //
const int mode_max = 2;       // [0,1,2]
int mode = mode_max;                 // spotlight, mirror, auto
bool mirror_state = false;    // do lamps mirror
bool initial_pos = true;      // do lamps need to move to initial position?
int hold_count = 0;
const int hold_thresh1 = 20;
int arm_state = 0;                // stow or operational, [0,1]
const int arm_state_max = 1;

// ------------------------------------------ MPU6050 ----------------------------------------- //
#include <Wire.h>
const int MPU_address = 0x68;               // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;        // variables for accelerometer, temp, & gyro raw data
float acX, acY, acZ, temp, gyX, gyY, gyZ;   // nomrmalized values
const float LSB_per_g = 16384.0;            // conversions
const float LSB_per_degC = 340;             // Temp not right

//  I2C Data Packet
const int bytes_per_register = 2;
const int num_registers = 7;
const int bytes_total =  num_registers * bytes_per_register;


// --------------------------- VOID SETUP -------------------- //
// --------------------------- VOID SETUP -------------------- //
void setup() 
{
 Serial.begin(115200); delay(200);
 Serial.println(" "); Serial.println(FileName); Serial.println(" ");  

 // --------- LED --------- //
 LEDstrip.begin();
 LEDstrip.setBrightness(100); //adjust brightness here
 LEDstrip.show(); // Initialize all pixels to 'off'

 // --------- pot ---------- //
 pinMode(pot_pin, INPUT);

 // ------- nRF ----------- //
  myRadio.begin();  
  myRadio.setChannel(115);              // 125 Possible channels, each channel can have up to 6 addresses. 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ); 
  // myRadio.openReadingPipe(0, address_RT); // Not recieving data
  myRadio.stopListening();
  delay(1000);

  // --------- MPU6050 ------------ //
  Wire.begin();
  Wire.beginTransmission(MPU_address);    // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B);                       // PWR_MGMT_1 register
  Wire.write(0);                          // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  // --------- wheel pot ---------- //
  pot_cal = pot_mid - analogRead(pot_pin);   // calibrate wheel pot
}


// -------------------------------- VOID LOOP ----------------------------- //
// -------------------------------- VOID LOOP ----------------------------- //
void loop() 
{
  joy1_sw = joy1.getSW(); // read joystick button
  pot_val = analogRead(pot_pin);
  
  // state select
  // if button is pushed in, count cycles
  if(joy1_sw == HIGH) {hold_count++;}
  else{hold_count = 0;}

  if(hold_count > hold_thresh1) 
    {
      arm_state++;
      hold_count = 0;
    }
  if(arm_state > arm_state_max) {arm_state = 0;}

  // -------------------- initialize position ---------------------- //
  if(initial_pos == true)
  {
    degs[0] = f_moveServo(target_x, degs[0]);
    degs[1] = f_moveServo(target_y, degs[1]);
    LEDstrip.setPixelColor(LED_index, 50, 50, 50); LEDstrip.show();
    if(target_x == degs[0] && target_y == degs[1]) {initial_pos = false;} 
  }
    
  // ---------------------------- Offline ---------------------------------  //
  if(arm_state == 0)    
  {
    target_x = deg_min;
    target_y = deg_max;
    brightness = 0;
    mirror_state = false;
    initial_pos = true;
    mode = mode_max;
    LEDstrip.setPixelColor(LED_index, 10, 10, 50); LEDstrip.show();
  }

  // ----------------------------- Online ------------------------------ //
  if(arm_state == 1)
  {
    // --------------- read button press ---------- //
    if(joy1_sw != joy1_sw_last)                         // if state changes
    {
      if(joy1_sw == HIGH)                               // if new state is high
      {
        mode++;                                         // update mode
        if(mode > mode_max) {mode = 0;}
        initial_pos = true;                             // move to initial position
        
        // Spotlight
        if(mode == 0)          
        {
          target_x = deg_mid;
          target_y = deg_mid;
          brightness = 255;
          mirror_state = false; 
        }
        
        // Mirror
        if(mode == 1)          
        {
          target_x = 40;
          target_y = 120;
          brightness = 255;
          mirror_state = true;
        }
  
        // Auto
        if(mode == 2)          
        {
          target_x = deg_mid;
          target_y = deg_mid;
          brightness = 255;
          mirror_state = false;
        }
      }
    } 
    joy1_sw_last = joy1_sw;       // current joy state is now last joy state 

    if(initial_pos == false)
    {
    // -------------------- Joysitck Modes ---------------------- //
        if(mode == 0) // Spotlight
        {
          mirror_state = false;  
          LEDstrip.setPixelColor(LED_index, 50, 0, 0); LEDstrip.show();
        }
        
        if(mode == 1) // mirror state
        {
          mirror_state = true;
          LEDstrip.setPixelColor(LED_index, 0, 50, 0); LEDstrip.show();
        }    
        
        if(mode == 0 || mode == 1)
        {
          // Read joystick
          joy1_x = -joy1.getX();  // [-1:1]
          joy1_y = -joy1.getY();
          // brightness = f_getBrightness();

          // Deadband for no motion
          joy1_x = f_deadband(joy1_x);
          joy1_y = f_deadband(joy1_y);
      
          // Get LED Brightness
          // f_LED(brightness);
      
          // calculate step
          // step_x = f_joyToStep(joy1_x);  // [-1:1] to [step_min:step_max]
          // step_y = f_joyToStep(joy1_y);
          step_x = joy1_x * 30.0;  // max joystick = 5 steps
          
          step_y = joy1_y * 30.0;
    
          target_x = degs[0] + step_x;
          target_y = degs[1] + step_y;
          
          degs[0] = f_moveServo(target_x, degs[0]);
          degs[1] = f_moveServo(target_y, degs[1]);
          // relative positioning
          
        }
      
        // ------------------- Accelerometer Mode -------------------- //
        /*
        if(mode == 2)
        {
          mirror_state = false;
          f_getData();                    // read from MPU6050
          acX = f_norm_ac(AcX);           // normalize  [G:G]
          acX = f_deadband(acX);          // add deadband
          acX = acX * 50;                 // scaling factor
          target_x = deg_mid - acX;       // abosolute positioning from 90
          degs[0] = f_moveServo(target_x, degs[0]);
          LEDstrip.setPixelColor(LED_index, 0, 0, 50); LEDstrip.show();

          // Set LEDs
          B = 0;
          if(acX > 0) {R = abs(acX) * 4; G = 0;}
          if(acX < 0) {G = abs(acX) * 4; R = 0;}
          if(acX == 0) {R = 0; G = 0;}
        }
        */
         // ------------------- Auto Mode -------------------- //
        if(mode == 2)
        {
          mirror_state = false;
          pot_val = analogRead(pot_pin) + pot_cal;                    
          pot_volt = f_voltRead(pot_pin, pot_Vin);                              
          pot_deg = f_linearMap(pot_volt, 0, pot_Vin, 0, pot_deg_max);
          pot_rot = f_linearMap(pot_volt, 0, pot_Vin, 0, pot_rot_max);
          wheel_rot = -1* (pot_rot - pot_rot_mid);                        

          lamp_pos = (wheel_rot/wheel_rot_max) * 180;    
          lamp_pos = f_bound(lamp_pos, -180, 180);
          
          

          target_x = deg_mid - lamp_pos + 45;                 // abosolute positioning from 90
          degs[0] = f_moveServo(target_x, degs[0]);
          LEDstrip.setPixelColor(LED_index, 0, 0, 50); LEDstrip.show();

          // Set LEDs
          B = 0;
          if(acX > 0) {R = abs(acX) * 4; G = 0;}
          if(acX < 0) {G = abs(acX) * 4; R = 0;}
          if(acX == 0) {R = 0; G = 0;}
        }
        

        
      } 
    } // end Online
    
  
  // --------------------- Data Prep ---------------------------// 
  if(mirror_state == true)       // X Lamps Mirror
  {
    int a = 90 - degs[0];
    degs[2] = 90 + a;
  }
  else {degs[2] = degs[0];}      // X Lamps Follow
  degs[3] = degs[1];             // Y Lamps Follow


  // Bound servo degree
  for(int i=0; i<size_degs; i++)
  {
    degs[i] = f_bound(degs[i], deg_min, deg_max);
  }
  
  // Add mechanical 0s
 for(int i=0; i<size_degs; i++)
  {
    degs[i] = degs[i] + mech0s[i];
  }
  
  
  // ---------- populate & transmit data packet ------- //
  // send servo data, pot data, switch data
  // transmit to both
  data_t.joy_sw = joy1_sw;          
  data_t.brightness = brightness;

  // transmit to lamp 1
  data_t.servo_x = degs[0];   // servo_x1_degree
  data_t.servo_y = degs[1];   // servo__y1_degree
  myRadio.openWritingPipe(address1_TR);   // Lamp 1
  myRadio.write(&data_t, sizeof(data_t));

  // transmit to lamp 2
  data_t.servo_x = degs[2];   // servo_x2_degree
  data_t.servo_y = degs[3];   // servo_y2_degree
  myRadio.openWritingPipe(address2_TR);   // Lamp 2
  myRadio.write(&data_t, sizeof(data_t));

  LEDstrip.setPixelColor(LED_index, R, G, B); LEDstrip.show();
  LEDstrip.show();

  // ------- print --------- //
  print_mode = 1;
  if(print_mode == 1)
  {
  Serial.print(FileName); Serial.print(" ");
  Serial.print("  holdcount_" ); Serial.print(hold_count); Serial.print(" ");
  Serial.print("  arm_" ); Serial.print(arm_state); Serial.print(" ");
  Serial.print("  mode_" ); Serial.print(mode); Serial.print(" ");
  Serial.print("  mirror_" ); Serial.print(mirror_state); Serial.print(" ");
  Serial.print("  initial_pos_"); Serial.print(initial_pos); Serial.print(" ");
  Serial.print("  joyX_" ); Serial.print(joy1_x); Serial.print(" ");  
  Serial.print("  joyY_" ); Serial.print(joy1_y); Serial.print(" ");  
  Serial.print("  joySW_" ); Serial.print(joy1_sw); Serial.print(" "); 
  Serial.print("  bri_" ); Serial.print(brightness); Serial.print(" ");
  Serial.print("  step_x_" ); Serial.print(step_x); Serial.print(" ");
  Serial.print("  step_y_" ); Serial.print(step_y); Serial.print(" ");
  Serial.print("  target,x1,x2_" ); Serial.print(target_x); Serial.print(" "); Serial.print(degs[0]); Serial.print(" "); Serial.print(degs[2]); Serial.print(" "); 
  Serial.print("  target,y1,y2_" ); Serial.print(target_y); Serial.print(" "); Serial.print(degs[1]); Serial.print(" "); Serial.print(degs[3]); Serial.print(" ");
  Serial.print("  pot_val_" ); Serial.print(pot_val); Serial.print(" ");
  }

  if(print_mode == 2)
  {
  Serial.print( "joySW: " ); Serial.print(joy1_sw); Serial.print(" "); 
  Serial.print( "  x: " ); 
  Serial.print(joy1_x); Serial.print(" ");
  Serial.print(step_x); Serial.print(" ");
  Serial.print(target_x); Serial.print(" ");
  Serial.print(degs[0]); Serial.print(" "); 
  Serial.print( "  y: " ); 
  Serial.print(joy1_y); Serial.print(" ");  
  Serial.print(step_y); Serial.print(" ");
  Serial.print(target_y); Serial.print(" "); 
  Serial.print(degs[1]); Serial.print(" "); 
  }

  if(print_mode == 3)
  {
  Serial.print(target_x); Serial.print(" ");
  Serial.print(degs[0]); Serial.print(" "); 
  Serial.print(target_y); Serial.print(" "); 
  Serial.print(degs[1]); Serial.print(" "); 
  }

  Serial.println("");
  delay(25);
}


// --------------------------- END VOID LOOP ------------------------ //
// --------------------------- END VOID LOOP ------------------------ //


int f_moveServo(int target_deg, int deg)
{
  if(target_deg > deg)                          // move positive
  {
    if(target_deg - deg >= 10) {deg = deg + step_max;}  // big step
    else{deg = deg + step_min;}                          // small step
  }

  if(target_deg < deg)                          // move negative
  {
    if(deg - target_deg >= 10) {deg = deg - step_max;}  // big step
    else{deg = deg - step_min;}                            // small step
  }
  return deg;                                   // updated servo degrees
}



// funcitons joy [-1.0:1.0] to servo step [-5:5]
float f_joyToServo(float joy) 
{
  float servo_step;
  // servo_step = slope1 * joy;                        // slope
  // servo_step = servo_step + ((deg_max+deg_min)/2);  // shift up to middle position
  servo_step = joy*4;
  return servo_step;                               // [servo_min:servo_max]
}

float f_getBrightness()
{
  float brightness;
  brightness = analogRead(pot_pin);   // read raw value [0:1023]
  brightness = brightness / 1023;     // normalize [0:1]
  brightness = brightness * 255;       // convert to pwm [0:255]
  return brightness;
}

float f_stepServo(int deg, int target_deg, int step_)
{
  if(deg > target_deg) {deg = deg - step_;}
  if(deg < target_deg) {deg = deg + step_;}
  deg = f_bound(deg, deg_min, deg_max);
  return deg;
}


// ---------- deadband -------- //
float f_deadband(float x)
{
 float x_max = 1.0;
 float y_max = 1.0;
 float x_dead = .25;
 float m = (y_max - 0)/(x_max - x_dead);    // slope (y2-y1)/(x2-x1)
 float b1 = 0 - m*(-x_dead);                // y-intercept for line 1
 float b2 = 0 - m*(x_dead);        
 float y;                                   // output
         
 if(x < -x_dead) {y = m*(x) + b1;}          // segment 1
 else if(x > x_dead) {y = m*(x) + b2;}      // segment 2
 else{y = 0;}                               // segment 0
 return y;
}


// ------- f_joyToStep -------- //
float deadband = .5; // no motion
float x1 = -deadband/2;
float x2 = deadband/2; 
float m = (0-step_min)/(x1-joy_min);
float b1 = step_min - m*joy_min;
float b2 = step_max - m*joy_max;
float f_joyToStep(float joy)
{
  float step_;
  if(joy < x1) {step_ = m*joy + b1;}
  if(joy > x1 && joy < x2) {step_ = 0;}
  if(joy > x2) {step_ = m*joy + b2;}
  return step_;
}


// ------------- LED functions ------------ //
// LED response to joystick data
void f_LED(int brightness)
{
  int LED_brightness = brightness/2;
  if(joy1_sw == HIGH) {R = 0; G = 0; B = 50;}
  else
  {
    R = (LED_brightness) + abs(joy1_x * LED_brightness); 
    G = (LED_brightness) + abs(joy1_y * LED_brightness);
    B = (LED_brightness);
  }
  LEDstrip.setPixelColor(LED_index, R, G, B);
  LEDstrip.show();
}


float f_norm_ac(float raw_ac)
{
  const float LSB_per_g = 16384.0;  // [least significant bit per G
  const float G = 9.81;             // [m/sec]
  float norm_ac; 
  norm_ac = raw_ac / LSB_per_g;     // [G]
  // norm_ac = norm_ac * G;            // [m/sec]
  return norm_ac;                   // [G] out put in Gs
}

void f_getData()
{
  Wire.beginTransmission(MPU_address); 
  Wire.write(0x3B);             // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]           
  Wire.endTransmission(false);  // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_address, bytes_total, true);  // request a total of 14 registers

  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  AcX = Wire.read()<<8|Wire.read();  // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read()<<8|Wire.read();  // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read()<<8|Wire.read();  // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  GyX = Wire.read()<<8|Wire.read();  // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  GyY = Wire.read()<<8|Wire.read();  // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read()<<8|Wire.read();  // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  Tmp = Wire.read()<<8|Wire.read();  // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
}
