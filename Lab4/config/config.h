#ifndef CONFIG_H
#define CONFIG_H

#include <linux/i2c-dev.h>

void config_audio_pll(int iic_index);

void config_audio_codec(int iic_index);

#endif