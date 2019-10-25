#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hdmi.h"

/* Error Code */
#define OPEN_HDMI_ERROR -1
#define OPEN_HDMI_SUCCESS 1

/* Sizes */
#define PIXEL_SIZE 3

// File descriptor
static int32_t file;

// This function opens the hdmi file
// @return - succes(1) or an error code (negative)
int32_t hdmi_open()
{
    file = open("/dev/ecen427_hdmi", O_RDWR); // open the file
    if (file == OPEN_HDMI_ERROR) // ii it's neg 1, it's bad
    {
        return OPEN_HDMI_ERROR;
    }

    return OPEN_HDMI_SUCCESS; // otherwise, good
}

// This function moves the cursor
void hdmi_seek(uint32_t bytes)
{
    lseek(file, bytes, SEEK_SET); // seeks
}

// This function reads from the screen
// @return - read from the screen
int32_t hdmi_read(uint32_t bytesToRead)
{
    char buffer[bytesToRead];
    return read(file, buffer, bytesToRead); // read
}

// This function writes to the screen
// @param pixel - this is the color we want to write to
// @param size - how big we want the write to be (a single pixel, a whole line, etc.)
void hdmi_write(char pixel[], uint16_t size)
{
    write(file, pixel, size); // 3 = pixel
}

void hdmi_close()
{
    close(file); // close the file
}

