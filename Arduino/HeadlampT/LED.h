#ifndef LED_h
#define LED_h


#include <Adafruit_NeoPixel.h>
#define ledPin 2
#define numLeds 1

int ledIndex = 0;
int R; // [0:255]
int G;
int B;

// int brightness;
Adafruit_NeoPixel ledStrip(numLeds, ledPin, NEO_GRB + NEO_KHZ800);


void ledOff()
{
ledStrip.setPixelColor(ledIndex, 0, 0, 0);
ledStrip.show();
}

void ledRed()
{
ledOff();
ledStrip.setPixelColor(ledIndex, 50, 0, 0); 
ledStrip.show();
}

void ledGreen()
{
ledOff();
ledStrip.setPixelColor(ledIndex, 0, 50, 0); 
ledStrip.show();
}

void ledBlue()
{
ledOff();
ledStrip.setPixelColor(ledIndex, 0, 0, 50); 
ledStrip.show();
}

void ledLightBlue()
{
  // LEDOff();
  ledStrip.setPixelColor(ledIndex, 0, 30, 50); 
  ledStrip.show();
}

void ledWhite()
{
ledOff();
ledStrip.setPixelColor(ledIndex, 50, 50, 50); 
ledStrip.show();
}

void ledBrightWhite()
{
ledOff();
ledStrip.setPixelColor(ledIndex, 100, 100, 100); 
ledStrip.show();
}

void ledBrightYellow()
{
ledOff();
ledStrip.setPixelColor(ledIndex, 200, 200, 0); 
ledStrip.show();
}

void ledYellow()
{
  ledOff();
  ledStrip.setPixelColor(ledIndex, 50, 50, 0);
  ledStrip.show();
}

void ledSetup()
{
 ledStrip.begin();
 ledStrip.setBrightness(100); //adjust brightness here
 ledStrip.show(); // Initialize all pixels to 'off'
 ledRed();
}
#endif
