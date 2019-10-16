#ifndef HDMI_H
#define HDMI_H

#include <stdint.h>
#include "../globals/globals.h"

int32_t open_hdmi();

int32_t read_hdmi();

void seek_hdmi(uint32_t bytes);

void write_hdmi(char pixel[], uint16_t size);

#endif