#ifndef USER_AUDIO_H
#define USER_AUDIO_H

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include "../config/config.h"
#include "../wav/wav.h"
#include "../audio/user_audio.h"
#include <stdbool.h>

// Number to see which sound file to play
#define SOUND_INVADER_DIE   0
#define SOUND_LASER         1
#define SOUND_PLAYER_DIE    2
#define SOUND_UFO_DIE       3
#define SOUND_UFO           4
#define SOUND_WALK_1        5
#define SOUND_WALK_2        6
#define SOUND_WALK_3        7
#define SOUND_WALK_4        8

#define FILE_NOT_FOUND (-1) // error checking
#define I2C_INDEX 0 // I2C index found in /dev/
#define SUPER_BIG 100000 // Just allocate big memory for the audio

// This function inits the audio by:
// 1) Opening the char driver /dev/
// 2) Config the Codec code
// 3) Reads in the .wav files
void user_audio_init();

// This wrapper function plays the sound and outputs to the headphones
// @param file - file of driver found in /dev/
// @param audio_data - the array read in from the .wav files
// @param size - just a huge size
void user_audio_play(uint32_t file, int32_t* audio_data, uint32_t size);

// This function plays the sound given which sound as specified by identifiers on top
// @param sound_num - a number between 1-8 to play that specified sound
void user_audio_play_sound(uint8_t sound_num);

#endif