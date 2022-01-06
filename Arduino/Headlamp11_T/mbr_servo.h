#ifndef mbr_servo.h
#define mbr_servo.h

#include <MBR_functions.h>



const int deg_min = 0;
const int deg_max = 180;
const int deg_mid = 180/2;

bool mirror_state = false;


int degs[4] = {deg_min, deg_max, deg_min, deg_max};       // servo array {x1,y1,x2,y2} initial position
int size_degs = sizeof(degs)/sizeof(degs[0]);
int steps[2] = {0, 0};                                  // step x, step y

int target_x;
int target_y;
int step_x;
int step_y;

const int step_max = 5;
const int step_min = 1;
const int step_thresh1 = 10;

int servo_x_deg;
int servo_y_deg;



// next servo position in direction of target
int stepServo(int deg, int target_deg, int step)
{
    if(deg > target_deg) {deg = deg - step;}
    if(deg < target_deg) {deg = deg + step;}
    deg = f_bound(deg, deg_min, deg_max);
    return deg;
}

// next servo positon in direction of target with variable step size
int moveServo(int deg, int target_deg)
{
  if(target_deg > deg)                                  // move positive
  {
    if(target_deg - deg >= step_thresh1) {deg = deg + step_max;}  // big step
    else{deg = deg + step_min;}                         // small step
  }

  if(target_deg < deg)                                  // move negative
  {
    if(deg - target_deg >= step_thresh1) {deg = deg - step_max;}  // big step
    else{deg = deg - step_min;}                         // small step
  }
  return deg;                              
}

#endif
