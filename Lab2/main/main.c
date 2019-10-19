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

static bool debounced = false; // Flag used to determine if debounce timer is done

// Reset all counters
void generic_reset_counters() {
    debounce_ctr  = 0;
    increment_ctr = 0;
    half_sec_ctr  = 0;
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
        debounced = true;
    }

    // The time will auto-increment if pressed for 1/2 second
    if (++half_sec_ctr >= HALF_SECOND) {
        // If the buttons val hasn't changed, still presssing
        if (buttons_val == new_buttons_val) {
            // Counter used to auto-increment
            if (++increment_ctr >= INCREMENT_MAX_VAL) {
                set_time();
                increment_ctr = 0;
            }
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
    generic_reset_counters();
    new_buttons_val = read_buttons(); // get the button value
    clear_button_interrupts(); // clear button interrupts
    if (debounced) {
        set_time(); // sets the time
        debounced = false;
    }
}

// This is invoked each time there is a change in switch state
void isr_switches()
{
    generic_reset_counters();
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

