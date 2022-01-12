#ifndef Wheel_h
#define Wheel_h
#define pot_pin A3

const int pot_min = 0;
const int pot_max = 800;
const int pot_mid = 400;
const int pot_Vin = 5;        //?????
const int pot_rot_max = 10;     
const int pot_rot_mid = 5;
const int pot_deg_max = pot_rot_max * 360;  // 3600

const float wheel_rot_max = 2.5;
const int wheel_pot_mid = 0;            //// remove


float wheel_cal;            // inital position of wheel
float wheel_rot;

int pot_val;            // position of steering pot
int pot_deg;
float pot_volt;
float pot_rot;    // rotations of pot

float lamp_pos; // relative lamp position from forward





float readWheelRot()
{
  pot_volt = f_voltRead(pot_pin, pot_Vin);                                            // read pot voltage [0:5]
  pot_rot = f_linearMap(pot_volt, 0, pot_Vin, 0, pot_rot_max) - pot_rot_mid;          // pot rotation [-5:5]
  wheel_rot = -1 * pot_rot;                                                           // wheel rotation [-2.5;2.5]
  return wheel_rot;   
}

void wheel_setup()
{
  pinMode(pot_pin, INPUT);
  wheel_cal = readWheelRot(); // set wheel at neutral to initiate
}


#endif
