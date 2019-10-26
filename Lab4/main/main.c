#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "../config/config.h"

#define FILE_NOT_FOUND (-1)
#define BUFFER_SIZE 10
#define I2C_INDEX 0

static uint32_t file;

int main()
{ 
    config_audio_pll(I2C_INDEX);
    config_audio_codec(I2C_INDEX);

    file = open("/dev/audio", O_RDWR);
    if (file == FILE_NOT_FOUND)
        printf("File not found\n\r");
    char buffer[BUFFER_SIZE];
    write(file, buffer, BUFFER_SIZE);
    read(file, buffer, BUFFER_SIZE);
}