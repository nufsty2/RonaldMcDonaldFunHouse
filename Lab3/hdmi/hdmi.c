#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

/* Error Code */
#define OPEN_HDMI_ERROR -1
#define OPEN_HDMI_SUCCESS 1

static int32_t file;

int32_t open_hdmi()
{
    file = open("/dev/ecen427_hdmi", O_RDWR);
    if (file == OPEN_HDMI_ERROR)
    {
        return OPEN_HDMI_ERROR;
    }

    return OPEN_HDMI_SUCCESS;
}

void seek(uint32_t bytes)
{
    lseek(file, bytes, SEEK_SET);
}

int32_t read_hdmi(uint32_t bytesToRead)
{
    char buffer[bytesToRead];
    return read(file, buffer, sizeof(buffer)-1);
}

void write_hdmi(char pixel[])
{
    write(file, pixel, sizeof(pixel));
}

void close_hdmi()
{
    close(file);
}

