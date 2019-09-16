#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

// Inits the GPIO hardware for the buttons
// devDevice: file path we are reading from
// Returns whether it was a success (1) or not (-1)
int32_t init_buttons(char devDevice[]);

// Reads the buttons
// Returns the bitmask of said read buttons
int32_t read_buttons();