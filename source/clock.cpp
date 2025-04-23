//
// Created by francie on 4/20/2025.
//

#include "force_compile.h"

#include "clock.h"
#include <stdio.h>
#include "Arduino.h"

namespace clock
{
static time_t clock_time_;

void init()
{
	// we'll just set the initial time to the compile time
	// this way, the arduino has the correct time for as long as it stays alive
	// and additionally, when the user does have to set the date again, it shouldn't be too far off

	get_compile_time(clock_time_);
	clock_time_.last_update_ms = millis();

	// CAN BE REMOVED
	clock_time_.seconds += 3; // offsetting for the average compile time on my machine. __TIME__ is when compile starts
}
void set_clock_time(const time_t& time)
{
	clock_time_ = time;
}
const char* get_clock_time_string()
{
	return time_to_string(clock_time_);
}

void update_clock_time()
{
	const auto curr_time = millis();
	const auto delta_ms = curr_time - clock_time_.last_update_ms;
	const auto delta_seconds = delta_ms / 1000;

	if (delta_seconds == 0)
	{
		return; // no new second yet. don't update
	}

	// use delta_seconds * 1000 here rather than the curr_time
	// if we use curr_time, we're going to drift.
	// ex: if the delta was 1020ms, we add 1 second to the clock. however, 1.02 seconds have actually passed.
	//     if we set last update to curr_time, then the next time we hit a delta of 1000ms, it would actually have been 1020ms since the last update
	clock_time_.last_update_ms += delta_seconds * 1000;

	clock_time_.seconds += delta_seconds; // initially just increment the seconds without rollover
	clock_time_.minutes += clock_time_.seconds / 60; // if there was rollover from the seconds, seconds / 60 will be added to the minutes
	// now take the remainder after division by 60 (if we have 65, that will turn into 05 as expected)
	// (if we have 125, this will also turn into 05 as expected)
	clock_time_.seconds %= 60;


	// same exact concept as above
	clock_time_.hours = (clock_time_.hours + (clock_time_.minutes / 60)) % 24;
	clock_time_.minutes %= 60;
}

const char* time_to_string(const time_t& time)
{
	static char buffer[16]; // only 16 long since we only have 16 cols to work with anyway
	snprintf(buffer, sizeof(buffer), "%02u:%02u:%02u", time.hours, time.minutes, time.seconds);
	return buffer;
}

void get_compile_time(time_t& out)
{
	// __TIME__ is formatted HH:mm:ss
	sscanf(__TIME__, "%02u:%02u:%02u", &out.hours, &out.minutes, &out.seconds); // %02u -- 2 unsigned ints
}
} // namespace clock