#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
        
/* Error codes */ 
#define INTC_SUCCESS 1 // if success for the INTC, return 1
#define INTC_OPEN_ERROR -1 // if it ain't a success, return -1
#define INTC_MMAP_ERROR -2 // MMAP error code

/* INTC Attributes */
#define INTC_MMAP_SIZE 0x10000 // found in /sys/class/uio/uio4/maps/map0
#define INTC_OFFSET 0 // found in /sys/class/uio/uio4/maps/map0 
#define INTC_ADDR 0x41800000 // found in /sys/class/uio/uio4/maps/map0

/* Useful offsets from the docs */
#define ISR_OFFSET 0x00
#define IER_OFFSET 0x08
#define IAR_OFFSET 0x0C
#define MER_OFFSET 0x1C
#define ILR_OFFSET 0x24

/* Size for the read */
#define REG_SIZE_BYTES 0x4

/* Enable interrupts */
#define UIO_ENABLE_INTS 0x1
#define ENABLE_BTN_SW_INTC 0x7 // for the irq
#define ENABLE_MER_BITS 0x3
#define DISABLE_MER_BITS 0x0

/* Global statics for this file */
static int32_t file; // this is the file descriptor that describes an open UIO device
static char *ptr; // this is the virtual address of the UIO device registers

// Initializes the driver (opens UIO file and calls mmap)
// devDevice: The file path to the uio dev file
// Returns: A negative error code on error, INTC_SUCCESS otherwise
// This must be called before calling any other intc_* functions
// DEV DEVICE SHOULD BE: /dev/uio4
int32_t intc_init(char devDevice[])
{
    file = open(devDevice, O_RDWR); // open the device, read and write

    if (file == INTC_OPEN_ERROR) // if it can't open file, return error
    {
        return INTC_OPEN_ERROR; //f ile descriptors have to be > 0 to be valid
    }

    // memory map the physical address of the hardware into virtual address space
    ptr = mmap(INTC_ADDR, INTC_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, file, INTC_OFFSET); // mmap - creates a new mapping in the virtual address
    if (ptr == MAP_FAILED) // if it fails, return error
    {
        return INTC_MMAP_ERROR;
    }

    intc_irq_enable(ENABLE_BTN_SW_INTC);

    return INTC_SUCCESS;
}

// write to a register of the UIO device
void generic_write(uint32_t offset, uint32_t value)
{
    *((volatile uint32_t *)(ptr + offset)) = value; // the address is cast as a pointer so it can be dereferenced
}

// read from a register of the UIO device
uint32_t generic_read(uint32_t offset)
{
    return *((volatile uint32_t *)(ptr + offset)); // reads from a register
}

// This function returns the interrupt value read from the ISR, this will give us what is enabled
// Returns the ISR bitmask
uint32_t intc_get_interrupt_val() 
{
    return generic_read(ISR_OFFSET); // reading from the ISR
} 
        
// This function will block until an interrupt occurs
// Returns: Bitmask of activated interrupts
uint32_t intc_wait_for_interrupt()
{   

    char buf[REG_SIZE_BYTES];
    return read(file, buf, REG_SIZE_BYTES);
}
        
// Acknowledge interrupt(s) in the interrupt controller
// irq_mask: Bitmask of structinterrupt lines to acknowledge.
void intc_ack_interrupt(uint32_t irq_mask)
{
    generic_write(IAR_OFFSET, irq_mask);
}
        
// Instruct the UIO to enable interrupts for this device in Linux
// (see the UIO documentation for how to do this)
void intc_enable_uio_interrupts()
{
    uint32_t uio_enable = UIO_ENABLE_INTS;
    write(file, &uio_enable, REG_SIZE_BYTES);
}
        
// Enable interrupt line(s)
// irq_mask: Bitmask of lines to enable
// This function only enables interrupt lines, ie, a 0 bit in irq_mask
//	will not disable the interrupt line
void intc_irq_enable(uint32_t irq_mask)
{
    generic_write(IER_OFFSET, irq_mask); // write the irq mask to the IER
    generic_write(MER_OFFSET, ENABLE_MER_BITS); // write a 011 to the MER
}
        
// Same as intc_irq_enable, except this disables interrupt lines
void intc_irq_disable(uint32_t irq_mask)
{
    generic_write(IER_OFFSET, irq_mask); // write the irq mask to the IER
    generic_write(MER_OFFSET, DISABLE_MER_BITS); // write a 00 to the MER
}

// Called to exit the driver (unmap and close UIO file)
void intc_exit()
{
    munmap(ptr, INTC_MMAP_SIZE); // munmap - system call deletes the mappings for the specified address range
    close(file);
}
