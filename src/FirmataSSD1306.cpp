/*
  FirmataSSD1306.cpp - Firmata library v0.1.0 - 2015-11-22
  Copyright (C) 2013 Norbert Truchsess. All rights reserved.
  Copyright (C) 2014 Nicolas Panel. All rights reserved.
  Copyright (C) 2015 Jeff Hoefs. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.

  ###TODO: Update library reference###
  Provide encoder feature based on PJRC implementation.
  See http://www.pjrc.com/teensy/td_libs_Encoder.html for more informations
*/

#include <ConfigurableFirmata.h>
#include <Encoder7Bit.h>
#include <string.h>
#include "FirmataSSD1306.h"
#include "SSD1306Spi.h"

static SSD1306Spi* oleds[MAX_OLEDS];

/* Constructor */
FirmataSSD1306::FirmataSSD1306()
{
	memset(oleds, 0, sizeof(oleds[0]) * MAX_OLEDS);
}

void FirmataSSD1306::attachOled(byte oledNum, byte pinCs, byte pinDc, byte pinRst)
{
	if (_isAttached(oledNum)) {
		Firmata.sendString("OLED Warning: OLED is ");
		Firmata.sendString("already attached.\n");
	} else {
		Firmata.setPinMode(pinCs, PIN_MODE_SPI);
		Firmata.setPinMode(pinDc, PIN_MODE_SPI);
		Firmata.setPinMode(pinRst, PIN_MODE_SPI);

		oleds[oledNum] = new SSD1306Spi();
		oleds[oledNum]->attach(pinCs, pinDc, pinRst);
	}
}

void FirmataSSD1306::detachOled(byte oledNum)
{
	if (_isAttached(oledNum))
	{
		oleds[oledNum]->detach();
		free(oleds[oledNum]);
		oleds[oledNum] = NULL;
	}
}

boolean FirmataSSD1306::handlePinMode(byte pin, int mode)
{
	if (IS_PIN_SPI(pin) || IS_PIN_DIGITAL(pin)) {
		if (mode == PIN_MODE_SPI) {
			// the user must call I2C_CONFIG to enable I2C for a device
			return true;
		}
		//else if (isI2CEnabled) {
		//	// disable i2c so pins can be used for other functions
		//	// the following if statements should reconfigure the pins properly
		//	if (Firmata.getPinMode(pin) == PIN_MODE_I2C) {
		//		disableI2CPins();
		//	}
		//}
	}
	return false;
}

void FirmataSSD1306::handleCapability(byte pin)
{
	if (IS_PIN_SPI(pin) || IS_PIN_DIGITAL(pin)) {
		Firmata.write(PIN_MODE_SPI);
		Firmata.write(1); // TODO: could assign a number to map to SCL or SDA
	}
}

/* Handle OLED_DATA (0x63) sysex commands
 * See protocol details in "examples/SimpleFirmataSSD1306/SimpleFirmataSSD1306.ino"
*/
boolean FirmataSSD1306::handleSysex(byte command, byte argc, byte *argv)
{
	if (command == OLED_DATA)
	{
		byte oledCommand, oledNum, pinCs, pinDc, pinRst, data, mode;

		oledCommand = argv[0];

		if (oledCommand == OLED_ATTACH)
		{
			oledNum = argv[1];
			pinCs = argv[2];
			pinDc = argv[3];
			pinRst = argv[4];
			if (Firmata.getPinMode(pinCs) == PIN_MODE_IGNORE
				|| Firmata.getPinMode(pinDc) == PIN_MODE_IGNORE
				|| Firmata.getPinMode(pinRst) == PIN_MODE_IGNORE)
			{
				Firmata.sendString("OLED_ATTACH: Invalid pins");
				return false;
			}
			attachOled(oledNum, pinCs, pinDc, pinRst);
			return true;
		}

		if (oledCommand == OLED_RESET)
		{
			oledNum = argv[1];
			resetOled(oledNum);
			return true;
		}

		if (oledCommand == OLED_WRITE_DISPLAY)
		{
			oledNum = argv[1];
			data = (argv[2] << 7) + argv[3];
			mode = (argv[4] << 7) + argv[5];
			writeDisplay(oledNum, data, mode);
			return true;
		}

		if (oledCommand == OLED_WRITE_DISPLAY_BUF)
		{
			oledNum = argv[1];

			byte buffer[64];

			int length = ((argc - 2) * 7) / 8;
			Encoder7Bit.readBinary(length, &argv[2], buffer);

			for (int i = 0; i < length; i += 2)
			{
				uint8_t data = buffer[i];
				uint8_t mode = buffer[i + 1];

				writeDisplay(oledNum, data, mode);
			}
			return true;
		}

		if (oledCommand == OLED_DETACH)
		{
			oledNum = argv[1];
			detachOled(oledNum);
			return true;
		}

		Firmata.sendString("OLED Error: Invalid command");
	}
	return false;
}

void FirmataSSD1306::reset()
{
	byte oled;
	for (oled = 0; oled < MAX_OLEDS; oled++)
	{
		detachOled(oled);
	}
}

boolean FirmataSSD1306::_isAttached(byte oledNum)
{
	return oledNum < MAX_OLEDS && oleds[oledNum];
}

void FirmataSSD1306::resetOled(byte oledNum)
{
	if (_isAttached(oledNum))
	{
		Serial.print("resetOled() OLED number: ");
		Serial.println(oledNum);

		oleds[oledNum]->reset();
	}
}

void FirmataSSD1306::writeDisplay(byte oledNum, byte data, byte mode)
{
	if (_isAttached(oledNum))
	{
		// Serial.print(data, HEX);
		// Serial.print('-');
		// Serial.print(mode, HEX);
		// Serial.print(' ');

		oleds[oledNum]->writeDisplay(data, mode);
	}
}