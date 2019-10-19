#ifndef GENERIC_H
#define GENERIC_H

#include "../globals/globals.h"

void generic_draw_sprite(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size, char color[]);

const uint32_t* generic_get_sprite_from_digit(char digit);

void generic_reset_counters();

#endif