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

#define SOUND_INVADER_DIE   0
#define SOUND_LASER         1
#define SOUND_PLAYER_DIE    2
#define SOUND_UFO_DIE       3
#define SOUND_UFO           4
#define SOUND_WALK_1        5
#define SOUND_WALK_2        6
#define SOUND_WALK_3        7
#define SOUND_WALK_4        8

#define FILE_NOT_FOUND (-1)
#define I2C_INDEX 0
#define SUPER_BIG 100000

void user_audio_init();

void user_audio_play(uint32_t file, int32_t* audio_data, uint32_t size);

void user_audio_play_sound(uint8_t sound_num);

#endif