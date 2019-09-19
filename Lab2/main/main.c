    #include <stdint.h>
    #include "../intc/intc.h"
    #include "../buttons/buttons.h"
    #include "../switches/switches.h"
    #include <stdio.h>

    #define FIT_MASK 0x1
    #define BTN_MASK 0x2
    #define SW_MASK 0x3
     
    // This is invoked in response to a timer interrupt.
    // It does 2 things: 1) debounce switches, and 2) advances the time.
    void isr_fit()
    {
    }
     
    // This is invoked each time there is a change in the button state (result of a push or a bounce).
    void isr_buttons()
    {
        // Read the state of the buttons
        // ... do something ...
        // Acknowledge the button interrupt
    }

    // Init function for the main file
    void init()
    {
        intc_init("/dev/uio4"); // init the interrupt controller driver
        init_buttons("/dev/uio1"); // Initialize buttons 
        init_switches("dev/uio2"); // inits the switches
    }
     
    int main() 
    {
        printf("calling init\n\r");
        init(); // INITS EVERYTHING
        printf("After init\n\r");
        
        // Enable interrupt output from buttons
        intc_enable_uio_interrupts();
        // Enable button and FIT interrupt lines on interrupt controller

        uint32_t ctr = 0;
     
        printf("about to go into loop\n\r");
        while(1) 
        {
            // Call interrupt controller function to wait for interrupt
            uint32_t num_interrupts = intc_wait_for_interrupt();

            uint32_t interrupt_values = intc_get_interrupt_val();
            if (interrupt_values & FIT_MASK) {
                if (++ctr == 100) {
                    printf(".\n\r");
                    ctr = 0;
                }
            }
     
            // Check which interrupt lines are high and call the appropriate ISR functions
            /*if (interrupts & ...)
              isr_fit();
            if (interrupts & ...)
              isr_buttons();*/
        }

        // EXIT
        buttons_exit();
        switches_exit();
        intc_exit();
    }

