//
// Created by francie on 4/22/2025.
//

#ifndef REMOTE_H
#define REMOTE_H

#include <stdint.h>

namespace remote
{
void init(uint8_t recv_pin);

/// @return signed integer corresponding to the last button pressed. -1 if it's not new
int8_t get_button_idx();

/// @return returns the corresponding digit for a button on the remote (if it has one). -1 otherwise
int8_t idx_to_digit(const uint8_t idx);

int8_t cmd_to_button_idx(const uint16_t cmd);
}

#endif //REMOTE_H
