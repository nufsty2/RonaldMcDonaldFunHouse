#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

// Inits GPIO hardware for the switches
// The parameter devDevice is the string (char array) of the location of the switches UIO file
// First, attempt to open the file and see if it exists or is good
// Second, map the virtual address to the regular address, this is ptr
// Returns either -1 or -2 if bad, 1 if good
// DEV DEVICE SHOULD BE: /dev/uio2
int32_t init_switches(char devDevice[]);

// Reads the switches and returns the bitmask of whichever was flipped
// Switches are read from the GPIO_DATA register rather than reading directly from the switches
// Returns the dereferenced virtual address of the offset - 0 if both low, 1 if SW0 is high, 2 if SW1 is high, 3 if both high
uint32_t read_switches();

// This is a generic write on the switches, we use it to write the interrupt enable registers
void enable_switches_interrupts();

// This function clears the switch interrupts by writing a 1 to the first bit to toggle
void clear_switches_interrupts();

// Close uio device after read/write operations
void switches_exit();