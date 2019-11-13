#ifndef WAV_H
#define WAV_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// This function reads the wav file
// @param array - this reads in an unitalized array
// @param input - this is the file path of the .wav (../something.wav)
// @return - returns the number of samples and array now has all the values
uint32_t wav_read_data(int32_t *array, char * input);

#endif