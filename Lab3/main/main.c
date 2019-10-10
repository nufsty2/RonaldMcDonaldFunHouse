#include "../draw/draw.h"

void move_player() 
{
    if (buttons_val == BTN_0) // move right
    {
        if ((current_pos_player+15) % NEW_LINE != 1839)
        {
            // draw_alien(tank_15x8, current_pos_player, 15, 8, PIXEL_SIZE_GLOBAL*2, black);

            draw_alien(block_2x8, current_pos_player, 2, 8, PIXEL_SIZE_GLOBAL*2, black);

            current_pos_player += PIXEL_SIZE_GLOBAL * 4;

            draw_alien(tank_15x8, current_pos_player, 15, 8, PIXEL_SIZE_GLOBAL*2, cyan);
        }
    }

    else if (buttons_val == BTN_1) // move left
    {
        if (current_pos_player % NEW_LINE != 0)
        {
            // draw_alien(tank_15x8, current_pos_player, 15, 8, PIXEL_SIZE_GLOBAL*2, black);
            draw_alien(block_2x8, current_pos_player+13*6, 2, 8, PIXEL_SIZE_GLOBAL*2, black);

            current_pos_player -= PIXEL_SIZE_GLOBAL * 4;

            draw_alien(tank_15x8, current_pos_player, 15, 8, PIXEL_SIZE_GLOBAL*2, cyan);
        }
    }
}

// BTN0: Increase letter
// BTN1: Decrease letter
// BTN3: Submit letter
void respond_to_press()
{
    if (buttons_val == BTN_0) 
    {
        char val = *get_selected_char(selected_char);

        if (++val > 'Z')
            *get_selected_char(selected_char) = 'A';
        else
            *get_selected_char(selected_char) += 1;
    }

    else if (buttons_val == BTN_1)
    {
        char val = *get_selected_char(selected_char);

        if (--val < 'A')
            *get_selected_char(selected_char) = 'Z';
        else
            *get_selected_char(selected_char) -= 1;
    }

    else if (buttons_val == BTN_3)
    {
        if (++selected_char > 2) {
           name_entered = true;
           score_t new_score = {.name = {char_0, char_1, char_2}, .value = score};
           scores_write_new_score(new_score);
           main_print_scores();
        }
    }

    if (selected_char <= 2) 
    {
        blink_cursor(true);
        half_sec_ctr = 0;
        blink = true;
    }   
}

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
    if ((++half_sec_ctr >= HALF_SECOND) && game_over && !name_entered) {
        blink_cursor(false);
        half_sec_ctr = 0;
    }

    // If the buttons val hasn't changed, still presssing
    if (buttons_val == new_buttons_val) {
        // Counter used to auto-increment
        if (++increment_ctr >= INCREMENT_MAX_VAL) {
            move_player();
            increment_ctr = 0;
        }
    }

    if ((++alien_move_ctr >= ALIEN_MOVE_MAX_VAL) && !game_over) {
        alien_move_ctr = 0;
        move_alien_army();
    }

    if ((++saucer_ctr >= SAUCER_MAX_VAL) && !game_over)
    {
        saucer_moving = true;
        saucer_ctr = 0;
    }
    move_saucer();
}


// This is invoked each time there is a change in the button state (result of a push or a bounce).
void isr_buttons()
{
    reset_counters();
    new_buttons_val = read_buttons(); // get the button value
    clear_button_interrupts(); // clear button interrupts
    
    if (debounced) 
    {
        if (!game_over)
            move_player(); 

        else if (game_over && !name_entered) 
            respond_to_press();

        else
            init_end_game();

        debounced = false;
    }
}

// This is invoked each time there is a change in switch state
void isr_switches()
{
    reset_counters();
    new_switches_val = read_switches(); // read the switches
    clear_switches_interrupts(); // clear the interrupts
}
     
int main() 
{
    // Init Everything
    init();
        
    // Enable all interrupts
    intc_enable_uio_interrupts();
    enable_button_interrupts();
    enable_switches_interrupts();

    fit_ctr = 0; // reset fit counter

    while(!name_entered) 
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