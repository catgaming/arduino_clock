//
// Created by francie on 4/20/2025.
//

#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>
#include <Arduino.h>

namespace clock
{
struct time_t
{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
};
struct date_t
{
	uint8_t day;
	uint8_t month;
	uint8_t year;
};
struct moment_t
{
	time_t time;
	date_t date;
};

void init();
void set_now(moment_t& moment);
const char* get_now_time_string();
const char* get_now_date_string();

void increment_time(time_t& out);

/// @return std::string reference formatted as HH:mm:ss (24 hour time)
const char* time_to_string(time_t& time);
/// @return std::string reference formatted as MM/dd/yyyy
const char* date_to_string(date_t& date);

const char* month_int_to_string(uint8_t month);
const uint8_t month_string_to_int(const char* month);

/// @param[out] time_t containing the time that the compiler was invoked (good enough)
void get_compile_time(time_t& out);
/// @return date_t containing the date that the compiler was invoked
void get_compile_date();
} // namespace clock

#endif //CLOCK_H
