#include "../arduino/Sensors.h"
#include <Arduino.h>


Sensors::BaseAnalogSensor::BaseAnalogSensor(const char* lbl, uint8_t pin):
	pin(pin), raw(0), voltage(0), value(0)
{
	strncpy(label, lbl, sizeof(label));
}


Sensors::BaseAnalogSensor::~BaseAnalogSensor()
{
}


void Sensors::BaseAnalogSensor::read(void)
{
	raw = analogRead(pin);
	voltage = (raw / 1024.0) * 5.0;
}


size_t Sensors::BaseAnalogSensor::printTo(Print& p) const
{
	size_t s = 0;

	s += p.print(label);
	s += p.print(" raw=");
	s += p.print(raw);
	s += p.print(" volt=");
	s += p.print(voltage);
	s += p.print(" value=");
	s += p.print(value);

	return s;
}


Sensors::Temperature::Temperature(uint8_t pin):
	BaseAnalogSensor("temperature", pin)
{

}


void Sensors::Temperature::read(void)
{
	BaseAnalogSensor::read();
	value = (voltage - 0.5) * 100;
}


Sensors::Light::Light(uint8_t pin):
	BaseAnalogSensor("light", pin)
{
}


void Sensors::Light::read(void)
{
	BaseAnalogSensor::read();
	value = voltage / 5.0;
}
