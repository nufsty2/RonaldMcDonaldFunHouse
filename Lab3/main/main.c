#include "../draw/draw_alien.h"
#include "../globals/globals.h"
#include "../sprites/sprites.c"

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
extern const uint32_t* alien_army_sprites[][11];
extern bool alien_army_is_alive[][11];
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

uint16_t draw_alien_get_y_coord(uint32_t coord) {
    return coord / NEW_LINE;
}

uint16_t draw_alien_get_x_coord(uint32_t coord, uint16_t y_coord) {
    return (coord - y_coord * NEW_LINE) / PIXEL_SIZE_GLOBAL;
}

void fire_bullet()
{
    // Declare variables
    uint32_t old_pos_bullet = current_pos_bullet;

    uint16_t y_coord = draw_alien_get_y_coord(current_pos_bullet);
    printf("Bullet coord = (%d, %d)\n\r", draw_alien_get_x_coord(current_pos_bullet, y_coord), y_coord);

    // Draw the bullet
    draw_alien(tankbullet_1x5, current_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL*2, cyan); // draw the bullet

    // Move bullet up
    old_pos_bullet = current_pos_bullet;
    current_pos_bullet -= (NEW_LINE*4); // make bullet go up

    draw_alien(tankbullet_gone_1x5, old_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL*2, black); // erase old bullet

    draw_alien(tankbullet_1x5, current_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL*2, cyan); // draw new bullet

    // At this point, bullet gone
    if (current_pos_bullet < (640 + (NEW_LINE * 10))) 
    {
        bullet_moving = false;
        draw_alien(tankbullet_1x5, current_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL*2, black); // erase old bullet
    }
}

void move_player_right()
{
    if ((current_pos_player+15) % NEW_LINE != 1839)
    {
        draw_alien(block_2x8, current_pos_player, 2, 8, PIXEL_SIZE_GLOBAL*2, black);

        current_pos_player += PIXEL_SIZE_GLOBAL * 4;

        draw_alien(tank_15x8, current_pos_player, 15, 8, PIXEL_SIZE_GLOBAL*2, cyan);
    }
}

void move_player_left()
{
    if (current_pos_player % NEW_LINE != 0)
    {
        draw_alien(block_2x8, current_pos_player+13*6, 2, 8, PIXEL_SIZE_GLOBAL*2, black);

        current_pos_player -= PIXEL_SIZE_GLOBAL * 4;

        draw_alien(tank_15x8, current_pos_player, 15, 8, PIXEL_SIZE_GLOBAL*2, cyan);
    }
}

void move_player()
{

    if (buttons_val == BTN_0)
        move_player_right();
    else if (buttons_val == BTN_1)
        move_player_left();
}

void draw_alien_detect_hit()
{

    for (uint32_t row = 0; row < NO_ALIEN_Y; row++)
    {
        // y positions
        uint32_t top_border = current_pos_alien + NEW_LINE * (row * (ALIEN_SPRITE_HEIGHT + MOVE_ROWS_DOWN_FOR_ALIENS));
        uint32_t bot_border = current_pos_alien + NEW_LINE * ((row + 1) * ALIEN_SPRITE_HEIGHT + row * MOVE_ROWS_DOWN_FOR_ALIENS);

        // coord_values
        uint16_t top_border_y = draw_alien_get_y_coord(top_border);
        uint16_t bot_border_y = draw_alien_get_y_coord(bot_border);


        for (uint32_t col = 0; col < NO_ALIEN_X; col++)
        {       
            // x positions
            uint32_t left_border = current_pos_alien + PIXEL_SIZE_GLOBAL * SIZE_SCALAR * (col * (ALIEN_SPRITE_WIDTH + SPACE_BW_ALIENS));
            uint32_t right_border = current_pos_alien + PIXEL_SIZE_GLOBAL * SIZE_SCALAR * ((col + 1) * ALIEN_SPRITE_WIDTH + col * SPACE_BW_ALIENS);

            // coord values
            uint16_t bullet_y = draw_alien_get_y_coord(current_pos_bullet);
            uint16_t bullet_x = draw_alien_get_x_coord(current_pos_bullet, bullet_y);
            uint16_t left_border_y = draw_alien_get_y_coord(left_border);
            uint16_t right_border_y = draw_alien_get_y_coord(right_border);
            uint16_t left_border_x = draw_alien_get_x_coord(left_border, left_border_y);
            uint16_t right_border_x = draw_alien_get_x_coord(right_border, right_border_y);            

            printf("%d <= X < %d)\n\r", left_border_x, right_border_x);
            printf("%d <= Y < %d)\n\r", top_border_y, bot_border_y);
            if ((bullet_x >= left_border_x) &&
                (bullet_x <  right_border_x) &&
                (bullet_y >= top_border_y) &&
                (bullet_y <  bot_border_y))
            {
                // Hit! Make an explosion
                printf("HIT!!!!!!!!!! ALIEN[%d][%d]\n\r", row, col);

                // Change sprite to empty and set boolean flag
                alien_army_is_alive[row][col] = false;
                alien_army_sprites[row][col] = alien_gone_13x8;

                bullet_moving = false;
                break;
            }
        }
    }
}

void move_alien_army() 
{
    int8_t move_distance;
    uint32_t old_pos = current_pos_alien;
    bool move_down;

    toggle_all_sprites();

    /* If an alien has reached the end */
    if ((current_pos_alien + (SPACE_MOVING_ALIENS + SPACE_BW_ALIENS) * SIZE_SCALAR * PIXEL_SIZE_GLOBAL * NO_ALIEN_X) % NEW_LINE == 0)
    {
        moving_right_alien = false;
        current_pos_alien += NEW_LINE * MOVE_ROWS_DOWN_FOR_ALIENS;
    }
    else if (current_pos_alien % NEW_LINE == 0)
    {
        moving_right_alien = true;
        current_pos_alien += NEW_LINE * MOVE_ROWS_DOWN_FOR_ALIENS;
    }
    
    move_distance = moving_right_alien ? (PIXEL_SIZE_GLOBAL * SIZE_SCALAR) : -(PIXEL_SIZE_GLOBAL * SIZE_SCALAR);
    move_down = (current_pos_alien != old_pos) ? true : false;
    old_pos = current_pos_alien;

    for (uint16_t alien_y = 0; alien_y < NO_ALIEN_Y; alien_y++)
    {  
        for (uint32_t height = 0; height < 8; height++)                                                                          
        {
            if (move_down)
            {   
                int16_t correction = (moving_right_alien) ? (NEW_LINE * MOVE_ROWS_DOWN_FOR_ALIENS) : (NEW_LINE * MOVE_ROWS_DOWN_FOR_ALIENS) - move_distance;
                draw_lots_o_aliens(current_pos_alien - correction, ALIEN_SPRITE_WIDTH, height, alien_y, PIXEL_SIZE_GLOBAL * SIZE_SCALAR, black, true);
            }
            draw_lots_o_aliens(current_pos_alien, ALIEN_SPRITE_WIDTH, height, alien_y, PIXEL_SIZE_GLOBAL * SIZE_SCALAR, magenta, false);
            current_pos_alien += NEW_LINE * SIZE_SCALAR;
            seek_hdmi(current_pos_alien);
        }
        current_pos_alien += NEW_LINE * MOVE_ROWS_DOWN_FOR_ALIENS;
        seek_hdmi(current_pos_alien);
    }
    current_pos_alien = old_pos + move_distance;
    seek_hdmi(current_pos_alien);
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

    if ((!game_over) && (bullet_moving)) // bullet firing
    {
         fire_bullet();
         draw_alien_detect_hit();
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