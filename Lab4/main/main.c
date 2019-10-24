#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

static uint32_t file;

int main()
{ 
    file = open("/dev/audio", O_RDWR);

    if (file == -1)
        printf("File not found\n\r");

    char buffer[10];

    write(file, buffer, 10);

    read(file, buffer, 10);
}