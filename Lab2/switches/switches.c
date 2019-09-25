#include "switches.h"

/* Errors/Successes */
#define SWITCH_OPEN_ERROR -1 // error if it can't get the switches UIO file to open
#define SWITCH_MMAP_ERROR -2 // error if it can't mmap it for some reason
#define SWITCH_SUCCESS 1 // if we good

/* Switch Attributes */
#define SWITCH_MMAP_SIZE 0x10000 // found in /sys/class/uio/uio2/maps/map0
#define SWITCH_OFFSET 0 // found in /sys/class/uio/uio2/maps/map0
#define SWITCH_ADDR 0x41230000 // found in /sys/class/uio/uio2/maps/map0

/* Useful offsets for the GPIOs found in documentatin */
#define GIER_OFFSET 0x011C // offset of the global interrupt enable register
#define IER_OFFSET 0x0128 // IP Interrupt Enable Register
#define ISR_OFFSET 0x0120 // IP Interrupt Status Register
#define GPIO_DATA_OFFSET 0x0000 // offset of the GPIO Data register

/* Bits to write */
#define WRITE_TO_BIT_1 0x1
#define WRITE_TO_BIT_32 0x80000000


static int32_t file; // this is the file descriptor that describes an open UIO device
static char *ptr; // this is the virtual address of the UIO device registers

// Inits GPIO hardware for the switches
// The parameter devDevice is the string (char array) of the location of the switches UIO file
// First, attempt to open the file and see if it exists or is good
// Second, map the virtual address to the regular address, this is ptr
// Returns either -1 or -2 if bad, 1 if good
// DEV DEVICE SHOULD BE: /dev/uio2
int32_t init_switches(char devDevice[])
{
    file = open(devDevice, O_RDWR); // open the device, read and write

    if (file == SWITCH_OPEN_ERROR) // if it can't open file, return error
    {
        return SWITCH_OPEN_ERROR; // file descriptors have to be > 0 to be valid
    }

    // memory map the physical address of the hardware into virtual address space
    ptr = mmap(SWITCH_ADDR, SWITCH_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, file, SWITCH_OFFSET); // mmap - creates a new mapping in the virtual address
    if (ptr == MAP_FAILED) // if it failes, return error
    {
        return SWITCH_MMAP_ERROR; // bad
    }

    return SWITCH_SUCCESS; // return good if button setup was correct
}

// Reads the switches and returns the bitmask of whichever was flipped
// Switches are read from the GPIO_DATA register rather than reading directly from the switches
// Returns the dereferenced virtual address of the offset - 0 if both low, 1 if SW0 is high, 2 if SW1 is high, 3 if both high
uint32_t read_switches()
{
    return *((volatile uint32_t *)(ptr + GPIO_DATA_OFFSET));
}

// This is a generic write on the switches, we use it to write the interrupt enable registers
void write_switches(uint32_t offset, uint32_t value)
{
    *((volatile uint32_t *)(ptr + offset)) = value;
}

// This function enables the switch interrupts by writing the the IER and the GIER
void enable_switches_interrupts()
{
    write_switches(IER_OFFSET, WRITE_TO_BIT_1); // write a 1 to the IER
    write_switches(GIER_OFFSET, WRITE_TO_BIT_32); // write a 1 to the 32nd bit of the GIER
}

// This function clears the switch interrupts by writing a 1 to the first bit to toggle
void clear_switches_interrupts()
{
    write_switches(ISR_OFFSET, WRITE_TO_BIT_1); // write 1 the ISR to toggle it
}

// Closes Switches UIO device
void switches_exit()
{
    munmap(ptr, SWITCH_MMAP_SIZE); // munmap - system call deletes the mappings for the specified address range
    close(file); // closes the file
}