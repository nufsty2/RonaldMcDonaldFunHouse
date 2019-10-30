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

// audio data rray
static int32_t audio_data[32000];

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
}

int main()
{ 
    init(); // inits what we want

    wav_read_data(audio_data, "../wav_files/invader_die.wav"); // TEST

    if (file != -1)
    {
        write(file, audio_data, 32000);
        //read(file, audio_data, 32000);
    }

}