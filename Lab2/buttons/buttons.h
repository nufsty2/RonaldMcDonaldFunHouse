#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

// Inits GPIO hardware for the buttons
// First, we open the file given the devDevice location, this returns either success or error
// Second, we use the value ptr to map the virtual address to regular address
// The only parameters id devDevice which is a string of where the buttons file is located
// Returns either an error: -1 or -2 or a success, 1
int32_t init_buttons(char devDevice[]);

// This function reads the value of the button from the GPIO_DATA register so we don't read directly from the buttons
// This returns the dereferenced value from the virtual address, which is the bitmask of the buttons
uint32_t read_buttons();

// This function enables the GPIO interrupts by writing to the IER and GIER registers
void clear_button_interrupts();

// close the UIO device after all read/write operations
void buttons_exit();