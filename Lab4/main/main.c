#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "../config/config.h"
#include "../wav/wav.h"

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
    select_line_in(I2C_INDEX); // play it through headphones
}

void parse_wav_file()
{
    /*return*/ wav_read_data("../wav_files/invader_die.wav"); // TEST
}

int main()
{ 
    init(); // inits what we want

    /*int32_t buffer[512] =*/ parse_wav_file();

    // write(file, buffer, 512);
    // read(file, buffer, 512);
}