//
// Created by francie on 4/20/2025.
//

#include "lcd.h"

namespace lcd
{
void init()
{
	lcd_.begin(16, 2); // width x height
}

void clear()
{
	lcd_.clear();
}
} // namespace lcd