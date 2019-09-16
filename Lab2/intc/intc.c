    #include <stdint.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/mman.h>
     
    /* Error codes */ 
    #define INTC_SUCCESS 1 // if success for the INTC, return 1
    #define UIO_ERROR -1 // if it ain't a success, return -1
    #define UIO_SUCCESS 1 // if it is a success, returns 1

    /* Sizes */
    #define UIO_MMAP_SIZE 0x1000 // size of the mmap - 8 bytes

    /* Useful offsets */
    #define MMAP_OFFSET 0 // offset of the mmap
    #define GIER_OFFSET 0x011C // offset of the global interrupt enable register
    
    /* Global statics for this file */
    static int file; // this is the file descriptor that describes an open UIO device
    static char *ptr; // this is the virtual address of the UIO device registers

    // Initializes the driver (opens UIO file and calls mmap)
    // devDevice: The file path to the uio dev file
    // Returns: A negative error code on error, INTC_SUCCESS otherwise
    // This must be called before calling any other intc_* functions
    // DEV DEVICE SHOULD BE: /dev/uio4
    int32_t intc_init(char devDevice[])
    {
        file = open(devDevice, O_RDWR); // open the device, read and write

        if (file == UIO_ERROR) // if it can't open file, return error
        {
            return UIO_ERROR; //f ile descriptors have to be > 0 to be valid
        }

        // memory map the physical address of the hardware into virtual address space
        ptr = mmap(NULL, UIO_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, file, MMAP_OFFSET); // mmap - creates a new mapping in the virtual address
        if (ptr == MAP_FAILED) // if it failes, return error
        {
            return UIO_ERROR;
        }

        /* put hardware setup here */

        return UIO_SUCCESS;
    }

    // write to a register of the UIO device
    void generic_write(uint32_t offset, uint32_t value)
    {
        *((volatile uint32_t *)(ptr + offset)) = value; // the address is cast as a pointer so it can be dereferenced
    }

    // read from a register of the UIO device
    uint32_t generic_read(uint32_t offset)
    {
        return *((volatile uint32_t *)(ptr + offset)); // reeads from a register
    }

    // close the UIO device
    //	this function must be called after all read/write operations are done
    //	to properly unmap the memory and close the file descriptor
    void generic_exit()
    {
        munmap(ptr, UIO_MMAP_SIZE); // munmap - system call deletes the mappings for the specified address range
        close(file);
    }

    // Called to exit the driver (unmap and close UIO file)
    void intc_exit()
    {

    }
     
    // This function will block until an interrupt occurs
    // Returns: Bitmask of activated interrupts
    uint32_t intc_wait_for_interrupt()
    {
        // USE select() HERE SOMEWHERE - IT WAITS FOR AN INTERRUPT
    }
     
    // Acknowledge interrupt(s) in the interrupt controller
    // irq_mask: Bitmask of interrupt lines to acknowledge.
    void intc_ack_interrupt(uint32_t irq_mask)
    {

    }
     
    // Instruct the UIO to enable interrupts for this device in Linux
    // (see the UIO documentation for how to do this)
    void intc_enable_uio_interrupts()
    {

    }
     
    // Enable interrupt line(s)
    // irq_mask: Bitmask of lines to enable
    // This function only enables interrupt lines, ie, a 0 bit in irq_mask
    //	will not disable the interrupt line
    void intc_irq_enable(uint32_t irq_mask)
    {

    }
     
    // Same as intc_irq_enable, except this disables interrupt lines
    void intc_irq_disable(uint32_t irq_mask)
    {
        
    }

