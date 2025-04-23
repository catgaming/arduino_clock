//
// Created by francie on 4/20/2025.
//

#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

namespace clock
{
struct time_t
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	unsigned long last_update_ms;
};

void init();
void set_clock_time(const time_t& time);
const char* get_clock_time_string();

void update_clock_time();

/// @return c string formatted as HH:mm:ss (24 hour time)
const char* time_to_string(const time_t& time);

/// @param[out] time_t containing the time that the compiler was invoked (good enough)
void get_compile_time(time_t& out);
} // namespace clock

#endif //CLOCK_H
