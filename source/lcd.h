//
// Created by francie on 4/20/2025.
//

#ifndef LCD_H
#define LCD_H

#include "LiquidCrystal.h"
#include <stdio.h>

namespace lcd
{
// this WOULD be a std::optional, but arduino-cli only supports up to C++17
inline auto lcd_ = LiquidCrystal(12, 11, 5, 4, 3, 2); // RS, E(nable), D4, D5, D6, D7 // pin order on the arduino that corresponds to those

void init();
void clear();

template <typename... t_args>
void print(const char* fmt, t_args&&... args)
{
	char buf[16];
	snprintf(buf, sizeof(buf), fmt, args...);
	lcd_.print(buf);
}

template <typename... t_args>
void print1(const char* fmt, t_args&&... args)
{
	lcd_.setCursor(0, 0);
	print(fmt, args...);
}

template <typename... t_args>
void print2(const char* fmt, t_args&&... args)
{
	lcd_.setCursor(0, 1);
	print(fmt, args...);
}

} // namespace lcd

#endif //LCD_H
