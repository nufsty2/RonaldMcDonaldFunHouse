#ifndef GENERIC_H
#define GENERIC_H

#include "../globals/globals.h"

// This is our draw sprite function we use everywhere except for drawing the alien
// @param sprite - the sprite we want to draw
// @param pos - the position we want to draw on the screen
// @param width - width of the sprite
// @param height - height of the sprite
// @param pixel_size - the scalar we want on the sprite
// @param color - color of the sprite
void generic_draw_sprite(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size, char color[]);

// This function gets a digit sprite from the digit we pass in
// @param digit - the digit we want the sprite for
// return - returns the corresponding sprite for that digit
const uint32_t* generic_get_sprite_from_digit(char digit);

// Reset all counters
void generic_reset_counters();

#endif