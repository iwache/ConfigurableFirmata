#ifndef SSD1306Spi_h
#define SSD1306Spi_h

#include <Arduino.h>

// Values for writeDisplay() mode parameter.
/** Write to Command register. */
#define SSD1306_MODE_CMD     0

class SSD1306Spi
{
public:
	SSD1306Spi();
	~SSD1306Spi(); // => never destroy in practice

	void attach(byte pinCs, byte pinDc, byte pinRst);
	void detach();

	void reset();
	void writeDisplay(byte data, byte mode);

private:
	byte _pinCs;
	byte _pinDc;
	byte _pinRst;
};


#endif