#include "switches.h"

/* Errors/Successes */
#define SWITCH_OPEN_ERROR -1 // error if it can't get the switches UIO file to open
#define SWITCH_MMAP_ERROR -2 // error if it can't mmap it for some reason
#define SWITCH_SUCCESS 1 // if we good

/* Switch Attributes */
#define SWITCH_MMAP_SIZE 0x10000 // found in /sys/class/uio/uio2/maps/map0
#define SWITCH_OFFSET 0 // found in /sys/class/uio/uio2/maps/map0
#define SWITCH_ADDR 0x41230000 // found in /sys/class/uio/uio2/maps/map0

/* Switch bitmasks */
#define LOW_BITMASK 0 // if both switches are low
#define SW0_BITMASK 1 // if switch 0 is high
#define SW1_BITMASK 2 // if switch 1 is high
#define HIGH_BITMASK 3 // if both switches are high

#define GIER_OFFSET 0x011C // offset of the global interrupt enable register
#define IER_OFFSET 0x0128 // IP Interrupt Enable Register
#define ISR_OFFSET 0x0120 // IP Interrupt Status Register
#define GPIO_DATA_OFFSET 0x0000 // offset of the GPIO Data register
#define GPIO_TRI_OFFSET 0x0004

static int file; // this is the file descriptor that describes an open UIO device
static char *ptr; // this is the virtual address of the UIO device registers

// Inits GPIO hardware for the switches
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

    /* put hardware setup here */

    return SWITCH_SUCCESS; // return good if button setup was correct
}

// Reads the switches and returns the bitmask of whichever was flipped
uint32_t read_switches()
{
    return *((volatile uint32_t *)(ptr + GPIO_DATA_OFFSET));
}

void write_switches(uint32_t offset, uint32_t value)
{
    *((volatile uint32_t *)(ptr + offset)) = value;
}

void enable_switches_interrupts()
{
    write_switches(IER_OFFSET, 0x1);
    write_switches(GIER_OFFSET, 0x80000000);
}

void clear_switches_interrupts()
{
    write_switches(ISR_OFFSET, 0x1);
}

// Closes Switches UIO device
void switches_exit()
{
    munmap(ptr, SWITCH_MMAP_SIZE); // munmap - system call deletes the mappings for the specified address range
    close(file); // closes the file
}