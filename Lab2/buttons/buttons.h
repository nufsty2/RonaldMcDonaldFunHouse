#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

// Inits the GPIO hardware for the buttons
// devDevice: file path we are reading from
// Returns whether it was a success (1) or not (-1)
int32_t init_buttons(char devDevice[]);

// Generic read on the buttons
uint32_t read_buttons();

void clear_button_interrupts();

// close the UIO device after all read/write operations
void buttons_exit();