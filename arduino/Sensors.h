#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <stdint.h>
#include <Printable.h>


namespace Sensors {

class BaseAnalogSensor: public Printable {
private:
	uint8_t pin;
	char label[32];

public:
	BaseAnalogSensor(const char* label, uint8_t pin);
	virtual ~BaseAnalogSensor();

	void read(void);

	size_t printTo(Print& p) const;

	int raw;
	float voltage;
	float value;
};


class Temperature: public BaseAnalogSensor {
public:
	Temperature(uint8_t pin);

	void read(void);
};

// Assumes 10kOhm
class Light: public BaseAnalogSensor {
public:
	Light(uint8_t pin);

	void read(void);
};

};

#endif // __SENSORS_H__
