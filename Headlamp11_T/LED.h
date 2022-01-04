#ifndef LED_H_
#define LED_H_

#include <Adafruit_NeoPixel.h>
#define led_pin 2
#define num_leds 1

int LED_index = 0;
int R; // [0:255]
int G;
int B;

int brightness;
Adafruit_NeoPixel LEDstrip(num_leds, led_pin, NEO_GRB + NEO_KHZ800);

void LED_setup()
{
 LEDstrip.begin();
 LEDstrip.setBrightness(100); //adjust brightness here
 LEDstrip.show(); // Initialize all pixels to 'off'
}

#endif
