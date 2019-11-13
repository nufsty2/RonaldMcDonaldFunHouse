#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <string.h>
#include "../config/config.h"
#include "../wav/wav.h"
#include "../audio/user_audio.h"


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
        user_audio_init(); // inits what we want

        user_audio_play_sound(SOUND_UFO);

        return 0;
}
