#include "../draw/draw_alien.h"
#include "../globals/globals.h"
#include "../scores/scores.h"

extern uint32_t debounce_ctr;
extern uint32_t buttons_val;
extern uint32_t new_buttons_val;
extern uint32_t switches_val;
extern uint32_t new_switches_val;
extern uint32_t fit_ctr;
extern uint32_t half_sec_ctr;
extern uint32_t increment_ctr;
extern uint32_t alien_move_ctr;
extern uint32_t saucer_ctr;
extern uint32_t current_pos_alien;
extern uint32_t current_pos_player;
extern uint32_t current_pos_bullet;
extern bool debounced;
extern bool blink;
extern bool game_over;
extern bool name_entered;
extern uint32_t score;
extern char char_0;
extern char char_1;
extern char char_2;
extern uint8_t selected_char;
extern bool saucer_moving;
extern bool bullet_moving;
extern char black[];
extern char green[];
extern char cyan[];
extern char magenta[];
extern bool moving_right_alien;
extern uint32_t die_ctr;
extern bool start_die_ctr;

uint16_t draw_alien_get_y_coord(uint32_t coord) 
{
    return coord / NEW_LINE;
}

uint16_t draw_alien_get_x_coord(uint32_t coord, uint16_t y_coord) 
{
    return (coord - y_coord * NEW_LINE) / PIXEL_SIZE_GLOBAL;
}

void move_player()
{

    if (buttons_val == BTN_0)
        move_player_right();
    else if (buttons_val == BTN_1)
        move_player_left();
}

// BTN0: Increase letter
// BTN1: Decrease letter
// BTN3: Submit letter
// void respond_to_press()
// {
//     if (buttons_val == BTN_0) 
//     {
//         char val = get_selected_char(selected_char);

//         if (++val > 'Z')
//             get_selected_char(selected_char) = 'A';
//         else
//             get_selected_char(selected_char) += 1;
//     }

//     else if (buttons_val == BTN_1)
//     {
//         char val = get_selected_char(selected_char);

//         if (--val < 'A')
//             get_selected_char(selected_char) = 'Z';
//         else
//             get_selected_char(selected_char) -= 1;
//     }

//     else if (buttons_val == BTN_3)
//     {
//         if (++selected_char > 2) {
//            name_entered = true;
//            score_t new_score = {.name = {char_0, char_1, char_2}, .value = score};
//            scores_write_new_score(new_score);
//            main_print_scores();
//         }
//     }

//     if (selected_char <= 2) 
//     {
//         blink_cursor(true);
//         half_sec_ctr = 0;
//         blink = true;
//     }   
// }

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
    if ((++half_sec_ctr >= HALF_SECOND) && game_over && !name_entered)
    {
        blink_cursor(false);
        half_sec_ctr = 0;
    }

    // If the buttons val hasn't changed, still presssing
    if (buttons_val == new_buttons_val) 
    {
        // Counter used to auto-increment
        if (++increment_ctr >= INCREMENT_MAX_VAL) 
        {
            // Move player
            move_player();

            // Fire bullet if statement
            if ((buttons_val == BTN_3) && !(bullet_moving)) 
            {
                bullet_moving = true;
                current_pos_bullet = (current_pos_player + 42) - NEW_LINE * 10; 
                fire_bullet(); // inital fire
            }

            increment_ctr = 0;
        }
    }

    if ((++alien_move_ctr >= ALIEN_MOVE_MAX_VAL) && !game_over)
    {
        alien_move_ctr = 0;
        move_alien_army();
    }

    if ((++saucer_ctr >= SAUCER_MAX_VAL) && !game_over)
    {
        saucer_moving = true;
        saucer_ctr = 0;
    }
    saucer_moving = move_saucer(saucer_moving);

    if ((!game_over) && (bullet_moving) && !start_die_ctr) // bullet firing
    {
         fire_bullet();
         start_die_ctr = draw_alien_detect_hit();
    }

    if (start_die_ctr) // once an alien gets hit, start the counter
    {
        if ((++die_ctr >= 30) && !game_over) // so we can show an explosion and then erase
        {
            erase_dead_aliens();
            die_ctr = 0;
            start_die_ctr = false;
        }
    }
}


// This is invoked each time there is a change in the button state (result of a push or a bounce).
void isr_buttons()
{
    reset_counters();
    new_buttons_val = read_buttons(); // get the button value
    clear_button_interrupts(); // clear button interrupts
    
    if (debounced) 
    {
        if (!game_over) {}
            //move_player(); 

        else if (game_over && !name_entered) {}
            //respond_to_press();

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