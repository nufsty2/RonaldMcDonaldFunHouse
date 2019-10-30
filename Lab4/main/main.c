#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "../config/config.h"
#include "../wav/wav.h"


#define FILE_NOT_FOUND (-1)
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
    //open_audio_driver();

    // config the codec
    config_audio_pll(I2C_INDEX);
    //config_audio_codec(I2C_INDEX);
    //select_line_in(I2C_INDEX); // play it through headphones
}

int32_t* parse_wav_file()
{
    return wav_read_data("../wav_files/invader_die.wav"); // TEST
}

int main()
{ 
    init(); // inits what we want

    //int32_t* audio_data = parse_wav_file(); // data to test

    if (file != -1)
    {
        // char buf[1] = {0x7F};
        // write(file, buf, 1);
        // read(file, buf, 1);
    }

}