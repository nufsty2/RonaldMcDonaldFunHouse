    #include <stdint.h>
    #include "intc.h"
    #include "../buttons/buttons.h"
     
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
     
    int main() 
    {    
        intc_init("/dev/uio4"); // init the interrupt controller driver
        init_buttons("/dev/uio1"); // Initialize buttons  
     
        // Enable interrupt output from buttons
        // Enable button and FIT interrupt lines on interrupt controller
     
        while(1) 
        {
            // Call interrupt controller function to wait for interrupt
            uint32_t interrupts = intc_wait_for_interrupt();
     
            // Check which interrupt lines are high and call the appropriate ISR functions
            /*if (interrupts & ...)
              isr_fit();
            if (interrupts & ...)
              isr_buttons();*/
        }
    }

