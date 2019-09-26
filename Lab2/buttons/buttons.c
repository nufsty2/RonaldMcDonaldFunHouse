#include "buttons.h"

/* Errors/Successes */
#define BUTTON_OPEN_ERROR -1 // different number for debugging
#define BUTTON_MMAP_ERROR -2 // different number for debugging
#define BUTTON_SUCCESS 1

/* Button attributes */
#define BUTTON_MMAP_SIZE 0x10000 // From the file /sys/class/uio/uio1/maps/map0 : 
#define BUTTON_OFFSET 0 // From the file /sys/class/uio/uio1/maps/map0 : offset is 0
#define BUTTON_ADDR 0x41220000 // From the file /sys/class/uio/uio1/maps/map0 : this is address

/* Offsets */
#define GIER_OFFSET 0x011C // offset of the global interrupt enable register
#define IER_OFFSET 0x0128 // IP Interrupt Enable Register
#define ISR_OFFSET 0x0120 // IP Interrupt Status Register
#define GPIO_DATA_OFFSET 0x0000 // offset of the GPIO Data register

/* Bit writing */
#define WRITE_TO_BIT_1 0x1 // write a 1 to first bit
#define WRITE_TO_BIT_32 0x80000000 // write a 1 to 32nd bit

/* Global statics used in this file */
static int32_t file; // this is the file descriptor that describes an open UIO device
static char *ptr; // this is the virtual address of the UIO device registers

// Inits GPIO hardware for the buttons
// First, we open the file given the devDevice location, this returns either success or error
// Second, we use the value ptr to map the virtual address to regular address
// The only parameters id devDevice which is a string of where the buttons file is located
// Returns either an error: -1 or -2 or a success, 1
// DEV DEVICE SHOULD BE: /dev/uio1
int32_t init_buttons(char devDevice[])
{
     file = open(devDevice, O_RDWR); // open the device, read and write

        if (file == BUTTON_OPEN_ERROR) // if it can't open file, return error
        {
            return BUTTON_OPEN_ERROR; //f ile descriptors have to be > 0 to be valid
        }

        // memory map the physical address of the hardware into virtual address space
        ptr = mmap(NULL, BUTTON_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, file, BUTTON_OFFSET); // mmap - creates a new mapping in the virtual address
        if (ptr == MAP_FAILED) // if it fails, return error
        {
            return BUTTON_MMAP_ERROR;
        }

        return BUTTON_SUCCESS; // return good if button setup was correct
}

// This function reads the value of the button from the GPIO_DATA register so we don't read directly from the buttons
// This returns the dereferenced value from the virtual address, which is the bitmask of the buttons
uint32_t read_buttons()
{
    return *((volatile uint32_t *)(ptr + GPIO_DATA_OFFSET)); 
}

// This function writes to the buttons and is local to this class only
// Parameter 1 is the offset to write to
// Parameter 2 is the value we want to write 
void write_buttons(uint32_t offset, uint32_t value)
{
    *((volatile uint32_t *)(ptr + offset)) = value;
}

// This function enables the GPIO interrupts by writing to the IER and GIER registers
void enable_button_interrupts()
{
    write_buttons(IER_OFFSET, WRITE_TO_BIT_1); // write a 1 to the 1st bit of IER per documentation
    write_buttons(GIER_OFFSET, WRITE_TO_BIT_32); // write a 1 to the 32nd bit of IER per documentation
}

// This function clears the button interrupts by toggling the ISR offset
void clear_button_interrupts()
{
    write_buttons(ISR_OFFSET, WRITE_TO_BIT_1); // toggle per documentation
}

// This function unmaps the memory location, we only call this if we're done
void buttons_exit()
{
    munmap(ptr, BUTTON_MMAP_SIZE); // munmap - system call deletes the mappings for the specified address range
    close(file); // close the file
}