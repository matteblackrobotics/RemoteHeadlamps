#ifndef Timer_h
#define Timer_h

long chipTime;

long startTime1;
long duration1 = 2000;

void timerSetup()
{
    startTime1 = millis();
}


bool checkTime(long startTime, long duration)
{
  bool state;
  long chipTime = millis();
  if(chipTime - startTime >= duration) {state = true;}
  else{state = false;}
  return state;
}


#endif