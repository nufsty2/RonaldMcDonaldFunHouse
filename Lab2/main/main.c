    #include <stdint.h>
    #include "../intc/intc.h"
    #include "../buttons/buttons.h"
    #include "../switches/switches.h"
    #include <stdio.h>

    #define FIT_MASK 0x1 // 001
    #define BTN_MASK 0x2 // 010
    #define SW_MASK  0x3 // 011

    uint32_t fit_ctr = 0;
    uint32_t second_ctr = 0;
     
    // This is invoked in response to a timer interrupt.
    // It does 2 things: 1) debounce switches, and 2) advances the time.
    void isr_fit()
    {
        if (++fit_ctr == 100)
        {
            second_ctr++;
            printf("\r%2d", second_ctr);
            fflush(stdout);
            fit_ctr = 0;
        }
    }
     
    // This is invoked each time there is a change in the button state (result of a push or a bounce).
    void isr_buttons()
    {
        printf("\r#");
        fflush(stdout);

        uint32_t buttonRead = read_buttons(0);

        if (buttonRead == 1)
        {
            printf("Button 0 Pressed\n");
        }
        if (buttonRead == 2)
        {
            printf("Button 1 Pressed\n");
        }
        if (buttonRead == 4)
        {
            printf("Button 2 Pressed\n");
        }
        if (buttonRead == 8)
        {
            printf("Button 3 Pressed\n");
        }
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
        init(); // INITS EVERYTHING
        
        // Enable interrupt output from buttons
        intc_enable_uio_interrupts();
        // Enable button and FIT interrupt lines on interrupt controller

        fit_ctr = 0;

        while(1) 
        {
            // Call interrupt controller function to wait for interrupt
            uint32_t num_interrupts = intc_wait_for_interrupt();
            uint32_t interrupt_value = intc_get_interrupt_val();
            // printf("\r ** Val: 0x%X", interrupt_value);
            // fflush(stdout);
            intc_ack_interrupt(interrupt_value);
            if (interrupt_value & FIT_MASK)
            {
                isr_fit();
            }

            if (interrupt_value & BTN_MASK)
            {
                This doesn't work because you need to enable button interrupts with the GPIO stuff
                isr_buttons();
            }

            intc_enable_uio_interrupts();
        }

        // EXIT
        buttons_exit();
        switches_exit();
        intc_exit();
    }

