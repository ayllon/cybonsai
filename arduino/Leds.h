#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>


namespace Leds {

class RGB {
private:
	uint8_t red_pin, green_pin, blue_pin;

public:
	enum Color {
		RED, GREEN, BLUE
	};

	RGB(uint8_t r, uint8_t g, uint8_t b);
	void set(uint8_t r, uint8_t g, uint8_t b);
};

};

#endif // __LED_H__
