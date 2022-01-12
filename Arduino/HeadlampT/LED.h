#ifndef LED_h
#define LED_h


#include <Adafruit_NeoPixel.h>
#define led_pin 2
#define num_leds 1

int LED_index = 0;
int R; // [0:255]
int G;
int B;

// int brightness;
Adafruit_NeoPixel LEDstrip(num_leds, led_pin, NEO_GRB + NEO_KHZ800);


void LEDOff()
{
LEDstrip.setPixelColor(LED_index, 0, 0, 0);
LEDstrip.show();
}

void LEDRed()
{
LEDOff();
LEDstrip.setPixelColor(LED_index, 50, 0, 0); 
LEDstrip.show();
}

void LEDGreen()
{
LEDOff();
LEDstrip.setPixelColor(LED_index, 0, 50, 0); 
LEDstrip.show();
}

void LEDBlue()
{
LEDOff();
LEDstrip.setPixelColor(LED_index, 0, 0, 50); 
LEDstrip.show();
}

void LEDPurple()
{
  // LEDOff();
  LEDstrip.setPixelColor(LED_index, 0, 30, 50); 
  LEDstrip.show();
}

void LEDWhite()
{
LEDOff();
LEDstrip.setPixelColor(LED_index, 50, 50, 50); 
LEDstrip.show();
}

void LEDBrightYellow()
{
LEDOff();
LEDstrip.setPixelColor(LED_index, 200, 200, 0); 
LEDstrip.show();
}

void LEDYellow()
{
  LEDOff();
  LEDstrip.setPixelColor(LED_index, 50, 50, 0);
  LEDstrip.show();
}

void LED_setup()
{
 LEDstrip.begin();
 LEDstrip.setBrightness(100); //adjust brightness here
 LEDstrip.show(); // Initialize all pixels to 'off'
 LEDRed();
}
#endif
