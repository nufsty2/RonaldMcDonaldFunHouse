#include "pitc.h"

/* File descriptor */
static uint32_t file;

void pitc_init()
{
    file = open("/dev/uio5", O_RDWR);
    if (file == -1)
        printf("PIT file not found\n\r");
}

