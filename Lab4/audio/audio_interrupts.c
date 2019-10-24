#include "audio_interrupts.h"
#include "audio.c"

static int32_t file;

void audio_interrupts_init()
{
    file = open("/dev/audio", O_RDWR); // open the device, read and write
    audio_write(0x10, 0x1);
}