#ifndef CONFIG_H
#define CONFIG_H

#include <linux/i2c-dev.h>

void config_audio_pll(int iic_index);

void config_audio_codec(int iic_index);

void select_line_in(int iic_index);

void play(unsigned int audio_mmap_size,
                     unsigned int* BufAddr, unsigned int nsamples, 
                     int uio_index, int iic_index);

#endif