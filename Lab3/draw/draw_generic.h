#ifndef DRAW_GENERIC_H
#define DRAW_GENERIC_H

#include "../globals/globals.h"
#include "../hdmi/hdmi.h"
#include "../sprites/sprites.c"

void draw_sprite(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size, char color[]);

const uint32_t* get_sprite_from_digit(char digit);

void reset_counters();

#endif