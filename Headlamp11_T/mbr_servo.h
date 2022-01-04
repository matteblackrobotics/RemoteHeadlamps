#ifndef mbr_servo.h
#define mbr_servo.h




int deg_min = 0;
int deg_max = 180;
int deg_mid = 180/2;

bool mirror_state = false;


int degs[4] = {deg_min, deg_max, deg_min, deg_max};       // servo array {x1,y1,x2,y2} initial position
int size_degs = sizeof(degs)/sizeof(degs[0]);
int steps[2] = {0, 0};                                  // step x, step y

int target_x;
int target_y;
int step_x;
int step_y;

int step_max = 5;
int step_min = 1;
int step_thresh1 = 10;

#endif
