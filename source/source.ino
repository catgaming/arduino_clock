#include "lcd.h"
#include "clock.h"
#include "Arduino.h"

void setup()
{
	lcd::init();
	clock::init();
}

void loop()
{
	lcd::print1(clock::get_clock_time_string());

	clock::update_clock_time();
	delay(100);
}