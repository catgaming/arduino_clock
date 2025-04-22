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
	lcd::print1(clock::get_now_time_string());
	lcd::print2(clock::get_now_time_string());

	clock::update_now();
	delay(100);
}