#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

// Inits the GPIO hardware for the switches
// devDevice: file path we are reading from
// Returns whether it was a success (1) or not (-*)
int32_t init_switches(char devDevice[]);

// Generic read on the switches
uint32_t read_switches(uint32_t offset);

// Close uio device after read/write operations
void switches_exit();