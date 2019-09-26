#include <stdint.h>
#include "../intc/intc.h"
#include "../buttons/buttons.h"
#include "../switches/switches.h"
#include <stdio.h>
#include <stdbool.h>

/* Which interrupts are enabled masks */
#define FIT_MASK 0x1 // 001
#define BTN_MASK 0x2 // 010
#define SW_MASK  0x4 // 100

/* Useful bitmasks for the switches */
#define BTN_0 0x1
#define BTN_1 0x2
#define BTN_2 0x4
#define SW_0  0x1

/* 
 values */
#define DEBOUNCE_MAX_VAL  5
#define HALF_SECOND       50
#define INCREMENT_MAX_VAL 10
#define FIT_1_SEC         100 // 10ms * 100  1000ms = 1s

/* Enum-like structure to determine which part to inc based on buttons and switches */
#define SEC 0
#define MIN 1
#define HR  2

/* Initial values for the clock, where we start */
#define INIT_SEC 0
#define INIT_MIN 0
#define INIT_HR  0

/* Rollover, min, and max values for the clock */
#define SEC_MAX 59 // second maximum
#define MIN_MAX 59 // minute maximum
#define HR_MAX  23 // hr maximum

/* Global statics needed for the class */
static uint32_t fit_ctr       = 0;
static uint32_t half_sec_ctr  = 0;
static uint32_t increment_ctr = 0;

static int32_t second_ctr = INIT_SEC; // second counter
static int32_t minute_ctr = INIT_MIN; // min counter
static int32_t hour_ctr   = INIT_HR;  // hour counter

static uint32_t debounce_ctr     = 0; // debounce counter
static uint32_t buttons_val      = 0; // buttons value
static uint32_t new_buttons_val  = 0; // new buttons value
static uint32_t switches_val     = 0; // switches value
static uint32_t new_switches_val = 0; // new switches value

// Reset all counters
void reset_counters() {
    debounce_ctr  = 0; // reset db counter
    increment_ctr = 0;
    half_sec_ctr  = 0;
}

// This is our main function to print the time on the console
void print_time() 
{
    printf("\r%02d:%02d:%02d", hour_ctr, minute_ctr, second_ctr); // main print
    fflush(stdout); // flush the output so it doesn't print each line
}

// This is our function to advance the time
// The unit paramater is what the unit we are incrementing
// The bool counting is to make sure that we advance the time when the button is not being pressed
void advance_time(int16_t unit, bool counting)
{
    /* Switch statement for what we are incrementing */
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

    fit_ctr = 0; // reset the fit counter

    if (second_ctr > SEC_MAX) // rollover
    {
        if (counting) // inc the minutes
            minute_ctr++;

        second_ctr = 0; // reset second counter
    }
        
    if (minute_ctr > MIN_MAX) // rollover
    {
        if (counting) // inc the hour
            hour_ctr++;

        minute_ctr = 0; // reset minute counter
    }

    if (hour_ctr > HR_MAX) // rollover
    {
        if (counting) // reset all
        {
            second_ctr = 0; // reset second counter
            minute_ctr = 0; // reset minute counter
        }
        hour_ctr = 0; // reset hour counter
    }

    print_time(); // print the time when all done
}

// This is our function to reverse the time meaning that when a button is pressed when the switch is down
// We dont need a bool because we only call this when a button is pressed
// Paramater unit is the unit we are incrementing, sec, min, or hour
void reverse_time(int16_t unit)
{
    /* Switch statement to decrement whatever the unit is */
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

    fit_ctr = 0; // reset the fit ctr

    if (second_ctr < 0) // rollover
    {
        second_ctr = SEC_MAX;
    }

    if (minute_ctr < 0) // rollover
    {
        minute_ctr = MIN_MAX;
    }

    if (hour_ctr < 0) // rollover
    {
        hour_ctr = HR_MAX;
    }

    print_time(); // print the time after we finish
}

// This is the set_time function that either advances or reverses the time based on the btn and switches
void set_time()
{
    // This if statement is to make sure two buttons aren't being pressed at the same tim
    if (buttons_val == (BTN_0 | BTN_1) ||
        buttons_val == (BTN_0 | BTN_2) ||
        buttons_val == (BTN_1 | BTN_2) ||
        buttons_val == (BTN_0 | BTN_1 | BTN_2))
        {
            return;
        }

    /* This switch reads the button value and sw value to determine to either advance or stop time */
    switch(buttons_val)
    {
        case BTN_0:
            if (switches_val & SW_0)
            {
                advance_time(SEC, false);
            }
            else
            {
                reverse_time(SEC);
            }
            break;
        case BTN_1:
            if (switches_val & SW_0) // ==
            {
                advance_time(MIN, false);
            }
            else
            {
                reverse_time(MIN);
            }
            break;
        case BTN_2:
            if (switches_val & SW_0)
            {
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
        debounce_ctr = 0; // reset debounce counter when max value hit
        buttons_val = new_buttons_val; // assign the buttons val
        switches_val = new_switches_val; // assign the switches vals
    }

    if (++half_sec_ctr >= HALF_SECOND) {
        if (buttons_val == new_buttons_val) {
            if (increment_ctr >= INCREMENT_MAX_VAL) {
                set_time();
                increment_ctr = 0;
            }
            increment_ctr++;
        }
    }

    // Advanced the time after 1 second
    if (++fit_ctr == FIT_1_SEC)
    {
        advance_time(SEC, true);
    }
}
     
// This is invoked each time there is a change in the button state (result of a push or a bounce).
void isr_buttons()
{
    reset_counters();
    new_buttons_val = read_buttons(); // get the button value
    clear_button_interrupts(); // clear button interrupts
    set_time(); // sets the time
}

// This is invoked each time there is a change in switch state
void isr_switches()
{
    reset_counters();
    new_switches_val = read_switches(); // read the switches
    clear_switches_interrupts(); // clear the interrupts
}

// Init function to init all different files
void init()
{
    intc_init("/dev/uio4"); // init the interrupt controller driver
    init_buttons("/dev/uio1"); // Initialize buttons 
    init_switches("/dev/uio2"); // inits the switches
}
     
int main() 
{
    init(); // INITS EVERYTHING
        
    // Enable all interrupts
    intc_enable_uio_interrupts();
    enable_button_interrupts();
    enable_switches_interrupts();

    fit_ctr = 0; // reset fit counter

    while(1) 
    {
        // Call interrupt controller function to wait for interrupt
        uint32_t num_interrupts = intc_wait_for_interrupt(); // wait
        uint32_t interrupt_value = intc_get_interrupt_val(); // get the ISR
            
        if (interrupt_value & FIT_MASK) // if fit is enabled go into isr_fit
        {
            isr_fit();
        }

        if (interrupt_value & BTN_MASK) // if button mask detected an interrupt, go into usr_buttons()
        {
            isr_buttons();
        }

        if (interrupt_value & SW_MASK) // if the switch detects an interrupt, go into isr_switches
        {
            isr_switches();
        }

        intc_ack_interrupt(interrupt_value); // acknowledge what was pressed
        intc_enable_uio_interrupts(); // re enables interrupts
    }

    // EXIT
    buttons_exit(); // exit the buttons
    switches_exit(); // exit the switches
    intc_exit(); // exits the int controller
}

