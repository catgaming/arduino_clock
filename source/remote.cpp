//
// Created by francie on 4/22/2025.
//

#include "remote.h"
#include "IRremote.h"

// from trial and error...... idk, there may have been an easier way
// the buttons are in this order on the remote:
//   69  70  71
//   68  64  67
//   07  21  09
//   22  25  13
//   12  24  94
//   08  28  90
//   66  82  74

// the layout is supposed to be like this on the remote:
//   00  01  02
//   04  05  06
//   08  09  10
//   12  13  14
//   16  17  18
//   20  21  22
//   24  25  26

// we'll use a struct table below to correspond raw cmd input to button idx

namespace remote
{
static int8_t last_idx = -1; // cache the button so we can ignore held down buttons (repeating signal)
static unsigned long last_ms = 0; // last time we reported a new button

// number of ms before a button can be pressed again
// the idea behind this: when a button is held down, repeated signals of the same idx are sent
// we don't care for held down buttons, so we only want to report a NEW button
// so we could just only return the button if it's different from the last button
// however, this ignores the case where you may want to press the same button twice
// this is what the interval is for
static constexpr uint16_t repeat_interval = 300; // number of ms before a button can be pressed again

void init(const uint8_t recv_pin)
{
	// start the IR receiver (no LED feedback)
	IrReceiver.begin(recv_pin, DISABLE_LED_FEEDBACK);
}

int8_t cmd_to_button_idx(const uint16_t cmd)
{
	// sorry about the mess. it's probably the most memory efficient way to do this though
	switch (cmd)
	{
		// col 1                    col 2                      col 3
	case  69: return  0; /**/ case  70: return  1; /**/ case  71: return  2;
	case  68: return  4; /**/ case  64: return  5; /**/ case  67: return  6;
	case   7: return  8; /**/ case  21: return  9; /**/ case   9: return 10;
	case  22: return 12; /**/ case  25: return 13; /**/ case  13: return 14;
	case  12: return 16; /**/ case  24: return 17; /**/ case  94: return 18;
	case   8: return 20; /**/ case  28: return 21; /**/ case  90: return 22;
	case  66: return 24; /**/ case  82: return 25; /**/ case  74: return 26;

	default:
		return -1;
	}
}

int8_t idx_to_digit(const uint8_t idx)
{
	switch (idx)
	{
		// col 1                  col 2                   col 3
	case 12: return 0;
	case 16: return 1; /**/ case 17: return 2; /**/ case 18: return 3;
	case 20: return 4; /**/ case 21: return 5; /**/ case 22: return 6;
	case 24: return 7; /**/ case 25: return 8; /**/ case 26: return 9;

	default:
		return -1;
	}
}

int8_t get_button_idx()
{
	int result = -1;

	// try to decode a new ir packet
	if (IrReceiver.decode()) {
		const auto cmd = IrReceiver.decodedIRData.command;
		const auto idx = cmd_to_button_idx(cmd);

		IrReceiver.resume();

		const auto curr_ms = millis();

		// return the button if it's a different button
		// or if it's the same but enough time has passed since it was last pressed
		if (idx >= 0 && (idx != last_idx || curr_ms - last_ms > repeat_interval))
		{
			last_idx = idx;
			last_ms = curr_ms;
			result = idx;
		}
	}

	return result;  // -1 if no new press, or the button index 0–26
}
}

