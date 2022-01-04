#ifndef joySW_h
#define joySW_h

int hold_count = 0;
int hold_thresh1 = 20;
bool armState = false;
bool joySW;

int mode = 0; // [0,1,2] spotlight, mirror, auto
int mode_last;
int mode_max = 3;
bool initialize;

bool sw;
bool sw_last;

// Is the system armed or not
bool checkArmState(bool sw)
  {
    if(sw == LOW) 
    {
      hold_count = 0; 
    }
    if(sw == HIGH) {hold_count++;}
    if(hold_count == hold_thresh1) 
    {
      armState = !armState;
    }
    if(armState == false) {mode = 0;}
    return armState;
  }


int checkMode(bool sw)
  {
    if(sw == HIGH)
    {
      if(sw != sw_last)
      {
        mode++;
        if(mode > mode_max) {mode = 1;}
      }
    }
    sw_last = sw;
    return mode;
  }


int checkMode1(bool sw)
{
  if(sw == LOW)
  {
    hold_count = 0;
  }

  if(sw == HIGH)
  {
    hold_count++;
    if(sw != sw_last)
    {
      mode++;
      if(mode > mode_max) {mode = 1;}
    }
  }

  if(hold_count == hold_thresh1)
  {
    mode = 0;
  }
}
  
#endif
