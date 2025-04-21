//
// Created by francie on 4/20/2025.
//

#ifndef LCD_H
#define LCD_H

#include "LiquidCrystal.h"

namespace lcd
{
// this WOULD be an std::optional, but arduino-cli only supports up to C++17
inline LiquidCrystal lcd_ = LiquidCrystal(12, 11, 5, 4, 3, 2); // RS, E(nable), D4, D5, D6, D7 // pin order on the arduino that coresponds to those

void init();
void clear();

template <typename... t_args>
void print1(const char* fmt, t_args&&... args)
{
	lcd_.setCursor(0, 0);
	lcd_.print(fmt, args...);
}

template <typename... t_args>
void print2(const char* fmt, t_args&&... args)
{
	lcd_.setCursor(0, 1);
	lcd_.print(fmt, args...);
}
} // namespace lcd

#endif //LCD_H
