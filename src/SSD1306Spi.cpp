
#include "SSD1306Spi.h"
#include <Arduino.h>
#include <SPI.h>

SSD1306Spi::SSD1306Spi()
{
}

SSD1306Spi::~SSD1306Spi() // => never destroy in practice
{
}

void SSD1306Spi::attach(byte pinCs, byte pinDc, byte pinRst)
{
	_pinCs = pinCs;
	_pinDc = pinDc;
	_pinRst = pinRst;

#if !defined(WIN32)
	pinMode(_pinCs, OUTPUT);
	digitalWrite(_pinCs, HIGH);
	pinMode(_pinDc, OUTPUT);
	pinMode(_pinRst, OUTPUT);
	SPI.begin();
#endif
}

void SSD1306Spi::detach()
{
}

void SSD1306Spi::reset()
{
#if !defined(WIN32)
	digitalWrite(_pinRst, LOW);
	delay(10);
	digitalWrite(_pinRst, HIGH);
	delay(10);
#endif
}

void SSD1306Spi::writeDisplay(byte data, byte mode)
{
#if !defined(WIN32)
	digitalWrite(_pinDc, mode != SSD1306_MODE_CMD);
	SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
	digitalWrite(_pinCs, LOW);
	SPI.transfer(data);
	digitalWrite(_pinCs, HIGH);
	SPI.endTransaction();
#endif
}
