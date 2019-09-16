#include "buttons.h"

#define BUTTON_ERROR -1
#define BUTTON_SUCCESS 1

#define BUTTON_MMAP_SIZE 0x1000 // 4 bytes for the button size
#define BUTTON_OFFSET 0 // I assume it's 0 for the offset?

static int file; // this is the file descriptor that describes an open UIO device
static char *ptr; // this is the virtual address of the UIO device registers

// inits GPIO hardware for the buttons
// DEV DEVICE SHOULD BE: /dev/uio1
int32_t init_buttons(char devDevice[])
{
     file = open(devDevice, O_RDWR); // open the device, read and write

        if (file == BUTTON_ERROR) // if it can't open file, return error
        {
            return BUTTON_ERROR; //f ile descriptors have to be > 0 to be valid
        }

        // memory map the physical address of the hardware into virtual address space
        ptr = mmap(NULL, BUTTON_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, file, BUTTON_OFFSET); // mmap - creates a new mapping in the virtual address
        if (ptr == MAP_FAILED) // if it failes, return error
        {
            return BUTTON_ERROR;
        }

        /* put hardware setup here */

        return BUTTON_SUCCESS; // return good if button setup was correct
}

int32_t read_buttons()
{

}