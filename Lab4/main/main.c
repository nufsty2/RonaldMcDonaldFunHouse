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

void open_audio_driver()
{
    file = open("/dev/audio", O_RDWR);
    if (file == FILE_NOT_FOUND)
        printf("File not found\n\r");
}

void init()
{
    // Open audio driver
    open_audio_driver();

    // config the codec
    config_audio_pll(I2C_INDEX);
    config_audio_codec(I2C_INDEX);
    //select_line_in(I2C_INDEX); // play it through headphones
}

void parseWAVFile()
{
    
}

int main()
{ 
    init(); // inits what we want

    // char buffer[BUFFER_SIZE];
    // write(file, buffer, BUFFER_SIZE);
    // read(file, buffer, BUFFER_SIZE);
}