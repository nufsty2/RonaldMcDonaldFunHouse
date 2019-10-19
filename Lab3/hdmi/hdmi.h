#ifndef HDMI_H
#define HDMI_H

#include <stdint.h>
#include "../globals/globals.h"

// This function opens the hdmi file
// @return - succes(1) or an error code (negative)
int32_t open_hdmi();

// This function reads from the screen
// @return - read from the screen
int32_t read_hdmi();

// this function moves the pointer to the screen to write/read to
// @param - number of bytes to seek
void seek_hdmi(uint32_t bytes);

// This function writes to the screen
// @param pixel - this is the color we want to write to
// @param size - how big we want the write to be (a single pixel, a whole line, etc.)
void write_hdmi(char pixel[], uint16_t size);

#endif