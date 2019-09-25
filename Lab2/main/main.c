    #include <stdint.h>
    #include "../intc/intc.h"
    #include "../buttons/buttons.h"
    #include "../switches/switches.h"
    #include <stdio.h>
    #include <stdbool.h>

    #define FIT_MASK 0x1 // 001
    #define BTN_MASK 0x2 // 010
    #define SW_MASK  0x4 // 100

    #define BTN_0 0x1
    #define BTN_1 0x2
    #define BTN_2 0x4
    #define SW_0  0x1

    #define DEBOUNCE_MAX_VAL 3

    #define SEC 0
    #define MIN 1
    #define HR  2

    uint32_t fit_ctr = 0;

    int32_t second_ctr = 10;
    int32_t minute_ctr = 1;
    int32_t hour_ctr = 00;

    uint32_t debounce_ctr = 0;
    uint32_t buttons_val = 0;
    uint32_t new_buttons_val = 0;
    uint32_t switches_val = 0;
    uint32_t new_switches_val = 0;

    void print_time() 
    {
        printf("\r%02d:%02d:%02d", hour_ctr, minute_ctr, second_ctr);
        fflush(stdout);
    }

    void advance_time(int16_t unit, bool counting)
    {
        switch (unit)
        {
        case SEC:
            second_ctr++;
            break;
        case MIN:
            minute_ctr++;
            break;
        case HR:
            hour_ctr++;
            break;
        default:
            break;
        }

        fit_ctr = 0;

        if (second_ctr > 59)
        {
            if (counting)
                minute_ctr++;

            second_ctr = 0;
        }
        
        if (minute_ctr > 59)
        {
            if (counting)
                hour_ctr++;

            minute_ctr = 0;
        }

        if (hour_ctr > 23)
        {
            if (counting)
            {
                second_ctr = 0;
                minute_ctr = 0;
            }
            hour_ctr = 0;
        }
        print_time();
    }

    void reverse_time(int16_t unit)
    {
        switch (unit)
        {
        case SEC:
            second_ctr--;
            break;
        case MIN:
            minute_ctr--;
            break;
        case HR:
            hour_ctr--;
            break;
        default:
            break;
        }

        fit_ctr = 0;

        if (second_ctr < 0) 
        {
            second_ctr = 59;
        }

        if (minute_ctr < 0) 
        {
            minute_ctr = 59;
        }

        if (hour_ctr < 0)
        {
            hour_ctr = 23;
        }
        print_time();
    }

    void set_time() {
        if (buttons_val == (BTN_0 | BTN_1) ||
            buttons_val == (BTN_0 | BTN_2) ||
            buttons_val == (BTN_1 | BTN_2) ||
            buttons_val == (BTN_0 | BTN_1 | BTN_2))
            {
                return;
            }

        switch(buttons_val) {
            case BTN_0:
                if (switches_val == SW_0) {
                    advance_time(SEC, false);
                }
                else
                {
                    reverse_time(SEC);
                }
                break;
            case BTN_1:
                if (switches_val == SW_0) {
                    advance_time(MIN, false);
                }
                else
                {
                    reverse_time(MIN);
                }
                break;
            case BTN_2:
                if (switches_val == SW_0) {
                    advance_time(HR, false);
                }
                else
                {
                    reverse_time(HR);
                }
                break;
            default:
                return;
        }
    }
     
    // This is invoked in response to a timer interrupt.
    // It does 2 things: 1) debounce switches, and 2) advances the time.
    void isr_fit()
    {
        if (++debounce_ctr >= DEBOUNCE_MAX_VAL)
        {
            debounce_ctr = 0;
            buttons_val = new_buttons_val;
            switches_val = new_switches_val;
        }

        // Advanced the time
        if (++fit_ctr == 100)
        {
            advance_time(SEC, true);
        }
    }
     
    // This is invoked each time there is a change in the button state (result of a push or a bounce).
    void isr_buttons()
    {
        debounce_ctr = 0;

        new_buttons_val = read_buttons();

        clear_button_interrupts();

        set_time();
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

