#ifndef Gimbal_h
#define Gimbal_h


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

#endif
