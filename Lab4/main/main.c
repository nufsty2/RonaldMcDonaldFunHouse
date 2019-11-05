#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include "../config/config.h"
#include "../wav/wav.h"


#define FILE_NOT_FOUND (-1)
#define I2C_INDEX 0
#define SUPER_BIG 100000

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
}

int main(int argc, char *argv[])
{ 
    if(argc < 2) 
    {
        printf("No sound file specified on command line!\n\r");
        return 1;
    }

    else
    {
        init(); // inits what we want

        for (uint16_t i = 1; i < argc; i++) {
            char file_path[100] = "../wav_files/";
            strcat(file_path, argv[i]);

            int32_t audio_data[SUPER_BIG];
            uint32_t size = wav_read_data(audio_data, file_path); // TEST

            for (uint8_t j = 0; j < 2; j++) 
            {
                write(file, audio_data, size);
                while(!read(file, audio_data, size));
            }
        }
        return 0;
    }
}