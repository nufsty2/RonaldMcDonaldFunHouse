    #include <stdint.h>
    #include "../intc/intc.h"
    #include "../buttons/buttons.h"
    #include "../switches/switches.h"
    #include <stdio.h>

    #define FIT_MASK 0x1 // 001
    #define BTN_MASK 0x2 // 010
    #define SW_MASK  0x3 // 011

    #define DEBOUNCE_MAX_VAL 3

    uint32_t fit_ctr = 0;
    uint32_t second_ctr = 0;
    uint32_t debounce_ctr = 0;
    uint32_t buttons_val = 0;
    uint32_t new_buttons_val = 0;
    uint32_t switches_val = 0;
    uint32_t new_switches_val = 0;
     
    // This is invoked in response to a timer interrupt.
    // It does 2 things: 1) debounce switches, and 2) advances the time.
    void isr_fit()
    {
        // Advanced the time
        if (++fit_ctr == 100)
        {
            second_ctr++;
            fit_ctr = 0;
        }

        if (++debounce_ctr >= DEBOUNCE_MAX_VAL)
        {
            debounce_ctr = 0;
            buttons_val = new_buttons_val;
            switches_val = new_switches_val;
        
            if (buttons_val == 1)
            {
                printf("\rButton 0 Pressed");
            }
            else if (buttons_val == 2)
            {
                printf("\rButton 1 Pressed");
            }
            else if (buttons_val == 4)
            {
                printf("\rButton 2 Pressed");
            }
            else if (buttons_val == 8)
            {
                printf("\rButton 3 Pressed");
            }
            else 
            {
                printf("\rNo button pressed!");
            }

            if (switches_val == 1)
            {
                printf("\rSwitch 1 activated!");
                fflush(stdout);
            }
            else if (switches_val == 2)
            {
                printf("\rSwitch 2 activated!");
                fflush(stdout);
            }
            else if (switches_val == 3)
            {
                printf("\rBoth SW activated!");
                fflush(stdout);
            }
            else
            {
                printf("\rNo switch activated!");
                fflush(stdout);
            }
        }
    }
     
    // This is invoked each time there is a change in the button state (result of a push or a bounce).
    void isr_buttons()
    {
        debounce_ctr = 0;

        new_buttons_val = read_buttons();
        clear_button_interrupts();
    }

    void isr_switches()
    {
        debounce_ctr = 0;

        new_switches_val = read_switches();
        clear_switches_interrupts();
    }

    // Init function for the main file
    void init()
    {
        intc_init("/dev/uio4"); // init the interrupt controller driver
        init_buttons("/dev/uio1"); // Initialize buttons 
        init_switches("/dev/uio2"); // inits the switches
    }
     
    int main() 
    {
        init(); // INITS EVERYTHING
        
        // Enable interrupt output from buttons
        intc_enable_uio_interrupts();
        enable_button_interrupts();
        enable_switches_interrupts();
        // Enable button and FIT interrupt lines on interrupt controller

        fit_ctr = 0;

        while(1) 
        {
            // Call interrupt controller function to wait for interrupt
            uint32_t num_interrupts = intc_wait_for_interrupt();
            uint32_t interrupt_value = intc_get_interrupt_val();
            // printf("\r ** Val: 0x%X", interrupt_value);
            // fflush(stdout);
            
            if (interrupt_value & FIT_MASK)
            {
                isr_fit();
            }

            if (interrupt_value & BTN_MASK)
            {
                // This doesn't work because you need to enable button interrupts with the GPIO stuff
                isr_buttons();
            }

            if (interrupt_value & SW_MASK)
            {
                isr_switches();
            }

            intc_ack_interrupt(interrupt_value);
            intc_enable_uio_interrupts(); // re enables interrupts
            // enable_switches_interrupts();
            // enable_button_interrupts();
        }

        // EXIT
        buttons_exit();
        switches_exit();
        intc_exit();
    }

