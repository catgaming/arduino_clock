//
// Created by francie on 4/20/2025.
//

#include "force_compile.h"

#include "clock.h"
#include <stdio.h>

namespace clock
{
static constexpr char* months_arr[] =
	{
	"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
static moment_t now_;

void init()
{
	// we'll just set the initial time to the compile time
	// this way, the arduino has the correct time for as long as it stays alive
	// and additionally, when the user does have to set the date again, it shouldn't be too far off

	get_compile_time(now_.time);
}
void set_now(moment_t& moment)
{
	now_ = moment;
}
const char* get_now_time_string()
{
	return time_to_string(now_.time);
}

const char* time_to_string(time_t& time)
{
	static char buffer[16]; // only 16 long since we only have 16 cols to work with anyway
	snprintf(buffer, sizeof(buffer), "%02u:%02u:%02u", time.hour, time.minute, time.second);
	return buffer;
}

const uint8_t month_string_to_int(const char* month)
{
	for (uint8_t i = 0; i < 12; i++)
	{
		if (strcmp(month, months_arr[i]) == 0)
		{
			return i + 1; // i + 1 bc we want to output a number that humans are used to
		}
	}

	return 0; // this should never get hit
}

void get_compile_time(time_t& out)
{
	// __TIME__ is formatted HH:mm:ss
	sscanf(__TIME__, "%02u:%02u:%02u", &out.hour, &out.minute, &out.second); // %02u -- 2 unsigned ints
}
void get_compile_date()
{

}
} // namespace clock