#include <stdio.h>
#include <stdint.h>

int main() {
    int32_t t1 = 0x12345678;
    int32_t t2 = (t1 & 0x00FF0000) << 8 | // Byte 1
                (~t1 & 0xFF000000) >> 8 | // Byte 2
                 (t1 & 0x000000FF) << 8 | // Byte 3
                 (t1 & 0x0000FF00) >> 8;  // Byte 4
    
    printf("t1: %X\n\r", t1);
    printf("t2: %X\n\r", t2);
}