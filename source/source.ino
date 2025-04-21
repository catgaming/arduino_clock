#include "lcd.h"
#include "clock.h"

void setup()
{
	lcd::init();
	clock::init();
}

void loop()
{
	lcd::print1("time");
	lcd::print2(clock::get_now_time_string());
	delay(1000);
}