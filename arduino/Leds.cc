#include "../arduino/Leds.h"
#include <Arduino.h>

Leds::RGB::RGB(uint8_t r, uint8_t g, uint8_t b):
	red_pin(r), green_pin(g), blue_pin(b)
{
	pinMode(red_pin, OUTPUT);
	pinMode(green_pin, OUTPUT);
	pinMode(blue_pin, OUTPUT);
}


void Leds::RGB::set(uint8_t r, uint8_t g, uint8_t b)
{
	analogWrite(red_pin, r);
	analogWrite(green_pin, g);
	analogWrite(blue_pin, b);
}
