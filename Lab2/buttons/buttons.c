#include "buttons.h"

/* Errors/Successes */
#define BUTTON_OPEN_ERROR -1 // different number for debugging
#define BUTTON_MMAP_ERROR -2 // different number for debugging
#define BUTTON_SUCCESS 1

/* Button attributes */
#define BUTTON_MMAP_SIZE 0x10000 // From the file /sys/class/uio/uio1/maps/map0 : 
#define BUTTON_OFFSET 0 // From the file /sys/class/uio/uio1/maps/map0 : offset is 0
#define BUTTON_ADDR 0x41220000 // From the file /sys/class/uio/uio1/maps/map0 : this is address

/* Button bitmasks */
#define BUTTON_0_BITMASK 1
#define BUTTON_1_BITMASK 2
#define BUTTON_2_BITMASK 4
#define BUTTON_3_BITMASK 8

/* Offsets */
#define GIER_OFFSET 0x011C // offset of the global interrupt enable register
#define IER_OFFSET 0x0128 // IP Interrupt Enable Register
#define ISR_OFFSET 0x0120 // IP Interrupt Status Register
#define GPIO_DATA_OFFSET 0x0000 // offset of the GPIO Data register
#define GPIO_TRI_OFFSET 0x0004

/* Global statics used in this file */
static int file; // this is the file descriptor that describes an open UIO device
static char *ptr; // this is the virtual address of the UIO device registers

// inits GPIO hardware for the buttons
// DEV DEVICE SHOULD BE: /dev/uio1
int32_t init_buttons(char devDevice[])
{
     file = open(devDevice, O_RDWR); // open the device, read and write

        if (file == BUTTON_OPEN_ERROR) // if it can't open file, return error
        {
            return BUTTON_OPEN_ERROR; //f ile descriptors have to be > 0 to be valid
        }

        // memory map the physical address of the hardware into virtual address space
        ptr = mmap(BUTTON_ADDR, BUTTON_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, file, BUTTON_OFFSET); // mmap - creates a new mapping in the virtual address
        if (ptr == MAP_FAILED) // if it fails, return error
        {
            return BUTTON_MMAP_ERROR;
        }

        /* put hardware setup here */

        return BUTTON_SUCCESS; // return good if button setup was correct
}

// This reads from the button and returns which one was readvia bitmask found in #defines
uint32_t read_buttons()
{
    return *((volatile uint32_t *)(ptr + GPIO_DATA_OFFSET)); 
}

void write_buttons(uint32_t offset, uint32_t value)
{
    *((volatile uint32_t *)(ptr + offset)) = value;
}

void enable_button_interrupts()
{
    write_buttons(IER_OFFSET, 0x1);
    write_buttons(GIER_OFFSET, 0x80000000);
}

void clear_button_interrupts()
{
    write_buttons(ISR_OFFSET, 0x1);
}

void buttons_exit()
{
    munmap(ptr, BUTTON_MMAP_SIZE); // munmap - system call deletes the mappings for the specified address range
    close(file);
}