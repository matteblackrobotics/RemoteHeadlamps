#ifndef Lamp_h
#define Lamp_h

int lampBrightness;


// map input 0-100 to outputs 1-255
int normalizeLamp(int lampIn)
{
    int lampOut;
    lampOut = linearMap(lampIn, 0, 100, 0, 255);
    return lampOut;
}

#endif
