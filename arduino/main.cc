#include <Arduino.h>

#include "../arduino/Leds.h"
#include "../arduino/Log.h"
#include "../arduino/Sensors.h"


void setup()
{
	Serial.begin(9600);
	// Turn off all leds
	for (int i = 2; i < 13; ++i) {
		pinMode(i, OUTPUT);
		digitalWrite(i, LOW);
	}
	pinMode(13, OUTPUT);
}


void loop()
{
	Sensors::Temperature temperature(A0);
	temperature.read();

	Log::data << temperature << Log::endl;

	Sensors::Light light(A1);
	light.read();

	Log::data << light << Log::endl;

	delay(1000);
}


int main(void)
{
	// Initialize Arduino
	init();

	// Setup internal context
	setup();

	// App loop
	for (;;) {
		digitalWrite(13, HIGH);
		loop();
		digitalWrite(13, LOW);
		loop();
	}

	return 0;
}
