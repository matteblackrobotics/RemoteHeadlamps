#ifndef wheel_h_
#define wheel_h_

#include <MBR_functions.h>


// --------------- Pot ---------------- //
#define wheel_pot_pin A3
int wheel_pot_cal;            // inital position of pot
int wheel_pot_val;            // position of steering pot
int wheel_pot_deg;
float wheel_pot_volt;

const int wheel_pot_min = 0;
const int wheel_pot_max = 800;
const int wheel_pot_mid = 400;
const int wheel_pot_Vin = 5;        //?????


float pot_rot;    // rotations of pot
const int pot_rot_max = 10;
const int pot_rot_mid = 5;
int pot_deg_max = pot_rot_max * 360;  // 3600


// ------------- wheel --------------- //
const float wheel_rot_max = 2.5;
float wheel_rot;


int readWheelPot()
{
  // wheel_pot_val = analogRead(wheel_pot_pin) + wheel_pot_cal;
  // wheel_pot_deg = f_linearMap(pot_volt, 0, Vin, 0, pot_deg_max);

  wheel_pot_volt = f_voltRead(wheel_pot_pin, Vin);
  wheel_pot_rot = f_linearMap(pot_volt, 0, pot_Vin, 0, pot_rot_max);
  wheel_rot = -1 * (pot_rot - pot_rot_mid);

}


void wheel_setup()
{
  pinMode(wheel_pot_pin, INPUT);
  wheel_pot_cal = wheel_pot_mid - readWheelPot(); // set wheel at neutral to initiate
}




#endif
