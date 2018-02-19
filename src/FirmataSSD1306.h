/*
  FirmataSSD1306.h - Firmata library
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

  FirmataSSD1306 handles instructions and is able to automatically report positions.
  See protocol details in "examples/SimpleFirmataSSD1306/SimpleFirmataSSD1306.ino"
*/

#ifndef FirmataSSD1306_h
#define FirmataSSD1306_h

#include <ConfigurableFirmata.h>
#include <FirmataFeature.h>

#define OLED_DATA               0x63 // control a SSD1306 OLED (proposal! belongs later on into line 76 of c:\Users\Vagrant\Source\Repos\libraries\ConfigurableFirmata\src\configurablefirmata.h)
#define PIN_MODE_SPI            0x0C // pin included in SPI setup (proposal! belongs later on into line 116 of c:\Users\Vagrant\Source\Repos\libraries\ConfigurableFirmata\src\configurablefirmata.h)

#define MAX_OLEDS                5 // arbitrary value, may need to adjust
#define OLED_ATTACH              (0x00)
#define OLED_RESET	             (0x01)
#define OLED_WRITE_DISPLAY       (0x02)
#define OLED_WRITE_DISPLAY_BUF   (0x03)
#define OLED_DETACH              (0x04)

class FirmataSSD1306 :public FirmataFeature
{
public:
	FirmataSSD1306();
	//~FirmataSSD1306(); => never destroy in practice

	// FirmataFeature implementation
	boolean handlePinMode(byte pin, int mode);
	void handleCapability(byte pin);
	boolean handleSysex(byte command, byte argc, byte *argv);
	void reset();

	// FirmataSSD1306 implementation
	boolean _isAttached(byte oledNum);
	void attachOled(byte oledNum, byte pinCs, byte pinDc, byte pinRst);
	void detachOled(byte oledNum);

	void resetOled(byte oledNum);
	void writeDisplay(byte oledNum, byte data, byte mode);

private:
};

#endif
