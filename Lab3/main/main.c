#include "../globals/globals.h"
#include "../scores/scores.h"
#include "../init/init.h"
#include "../hdmi/hdmi.h"
#include "../draw/alien.h"
#include "../draw/ui.h"
#include "../draw/bunker.h"
#include "../draw/generic.h"
#include "../draw/game_over.h"
#include "../draw/player.h"
#include "../../Lab2/buttons/buttons.h"
#include "../../Lab2/switches/switches.h"
#include "../../Lab4/audio/user_audio.h"
#include <sys/ioctl.h>

/* Defines for the main only */
#define LAST_CHAR 2
#define RANDOM_SCALAR 10
#define GUN_OFFSET 42
#define HEIGHT_OF_TANK_FOR_BULLET 10
#define DIE_CTR_MAX 30
#define WR_VALUE _IOW(242, 242, int32_t*) // 242 = major number

#define INC_VOL 1
#define DEC_VOL 0

/* Buttons and Switches */
extern uint32_t debounce_ctr;
extern uint32_t buttons_val;
extern uint32_t new_buttons_val;
extern uint32_t switches_val;
extern uint32_t new_switches_val;
extern bool debounced;

/* Ctrs for the FIT */
extern uint32_t fit_ctr;
extern uint32_t half_sec_ctr;
extern uint32_t increment_ctr;

/* Ctrs for the alien/saucer */
extern uint32_t alien_move_ctr;
extern uint32_t saucer_ctr;
uint32_t alien_bullet_ctr = 0;
extern uint32_t sound_invader_die_ctr;
extern uint32_t sound_player_die_ctr;
extern uint32_t sound_ufo_die_ctr;

/* Positions */
extern uint32_t current_pos_alien;
extern uint32_t current_pos_player;
extern uint32_t current_pos_bullet;

/* Game over */
extern bool blink;
extern bool game_over;
extern bool name_entered;
extern uint32_t score;
extern char char_0;
extern char char_1;
extern char char_2;
extern char selected_char;

/* Bools to see what is moving */
extern bool saucer_moving;
extern bool bullet_moving;
extern bool moving_right_alien;
extern bool player_dead;

/* Colors */
extern char black[];
extern char green[];
extern char cyan[];
extern char magenta[];

/* Player attributes */
extern uint8_t lives;

/* Once an alien is hit, start this counter to show the explosion and then erase */
extern uint32_t die_ctr;
extern bool start_die_ctr;
uint32_t player_death_ctr = 0;

/*File global for audio file*/ 
static uint32_t audio_file;

void adjust_volume()
{
    if (switches_val & GLOBALS_SW_0)
    {
        printf("Incrementing Volame\n\r");
        ioctl(audio_file, WR_VALUE, INC_VOL);
    }
    else 
    {
        printf("Dec Volame\n\r");
        ioctl(audio_file, WR_VALUE, DEC_VOL);
    }
}


// This function responds to button presses for the gameover
// BTN0: Increase letter
// BTN1: Decrease letter
// BTN2: Inc/Dec vols
// BTN3: Submit letter
void respond_to_press()
{
    if (buttons_val == GLOBALS_BTN_0) 
    {
        char val = *ui_get_selected_char();

        if (++val > 'Z')
            *ui_get_selected_char() = 'A';
        else
            *ui_get_selected_char() += 1;
    }

    else if (buttons_val == GLOBALS_BTN_1)
    {
        char val = *ui_get_selected_char();

        if (--val < 'A')
            *ui_get_selected_char() = 'Z';
        else
            *ui_get_selected_char() -= 1;
    }

    else if (buttons_val == GLOBALS_BTN_3)
    {
        if (++selected_char > LAST_CHAR) {
           name_entered = true;
           score_t new_score = {.name = {char_0, char_1, char_2}, .value = score};
           scores_write_new_score(new_score);
           game_over_print_scores();
        }
    }

    if (selected_char <= LAST_CHAR) 
    {
        game_over_blink_cursor(true);
        half_sec_ctr = 0;
        blink = true;
    }   
}

// Increments the sound counters
void increment_sound_ctrs() {
    // Increment counters
    if (sound_invader_die_ctr > 0)
    {
        if (++sound_invader_die_ctr >= SOUND_INVADER_DIE_CTR_MAX)
        {
            sound_invader_die_ctr = 0;
        }
    }
    if (sound_player_die_ctr > 0)
    {
        if (++sound_player_die_ctr >= SOUND_PLAYER_DIE_CTR_MAX)
        {
            sound_player_die_ctr = 0;
        }
    }
    if (sound_ufo_die_ctr > 0)
    {
        if (++sound_ufo_die_ctr >= SOUND_UFO_DIE_CTR_MAX)
        {
            sound_ufo_die_ctr = 0;
        }
    }
}

// This is our ISR FIT function that gets called every 10 ms
// This is our main controller for drawing and where our state machine is located
void isr_fit()
{
    if (!game_over) 
    {
        // Check first if the player lives are zero
        if (lives == 0) 
        {
            init_end_game();
            return;
        }

        increment_sound_ctrs();

        // If the player is dead, this is our player death animation
        if (player_dead)
        {
            if (++player_death_ctr >= GLOBALS_PLAYER_DEATH_MAX_VAL) // reset
            {
                current_pos_player = GLOBALS_PLAYER_START_POS;
                alien_draw(tank_15x8, current_pos_player, GLOBALS_TANK_WIDTH, GLOBALS_TANK_HEIGHT, GLOBALS_PIXEL_SIZE * GLOBALS_SIZE_SCALAR, cyan);
                player_dead = false;
                player_death_ctr = 0;
            }
            else if (++player_death_ctr >= GLOBALS_PLAYER_DEATH_STAGE_3) // animate
            {
                alien_draw(tank_gone_15x8, current_pos_player, GLOBALS_TANK_WIDTH, GLOBALS_TANK_HEIGHT, GLOBALS_PIXEL_SIZE * GLOBALS_SIZE_SCALAR, cyan);
            }
            else if (++player_death_ctr >= GLOBALS_PLAYER_DEATH_STAGE_2) // animate
            {
                alien_draw(tank_explosion2_15x8, current_pos_player, GLOBALS_TANK_WIDTH, GLOBALS_TANK_HEIGHT, GLOBALS_PIXEL_SIZE * GLOBALS_SIZE_SCALAR, cyan);
            }
            else if (++player_death_ctr >= GLOBALS_PLAYER_DEATH_STAGE_1) // animate
            {
                alien_draw(tank_explosion1_15x8, current_pos_player, GLOBALS_TANK_WIDTH, GLOBALS_TANK_HEIGHT, GLOBALS_PIXEL_SIZE * GLOBALS_SIZE_SCALAR, cyan);
            }
        }


        if ((++alien_bullet_ctr >= (rand() % GLOBALS_ALIEN_BULLET_MAX_VAL + GLOBALS_ALIEN_BULLET_MIN_VAL) * RANDOM_SCALAR)) 
        {
            alien_bullet_ctr = 0;
            alien_trigger_bullets();
        }

        // If we don't game over, always check if anything (bunkers and player) is hit
        alien_fire_bullets();
        bunker_detect_hits();
        if (!player_dead) player_detect_alien_hit();

        // This is our saucer move
        if ((++saucer_ctr >= GLOBALS_SAUCER_MAX_VAL))
        {
            saucer_moving = true;
            saucer_ctr = 0;
        }
        saucer_moving = alien_move_saucer(saucer_moving); // move the saucer if the flag is set

            // This is the player firing a bullet
        if (bullet_moving && !start_die_ctr) // bullet firing
        {
            player_fire_bullet(); // this is our bullet firing
            start_die_ctr = alien_detect_hit_army(); // if we get a hit (return true), start the die ctr
            if (saucer_moving) // if the saucer is moving, continously check for a hit on the saucer
            {
                alien_detect_hit_saucer();
            }
        }

        // This moves the aliens at a fixed rate
        if ((++alien_move_ctr >= GLOBALS_ALIEN_MOVE_MAX_VAL))
        {
            alien_move_ctr = 0;
            alien_move_army(); // have a counter that moves the alien army
        }    
    }
    
    // This is our debouncer for buttons and switches
    if (++debounce_ctr >= GLOBALS_DEBOUNCE_MAX_VAL)
    {
        debounce_ctr = 0; // reset debounce counter when max value hit
        buttons_val = new_buttons_val; // assign the buttons val
        switches_val = new_switches_val; // assign the switches vals
        debounced = true;
    }

    // If the buttons val hasn't changed, still presssing
    if (buttons_val == new_buttons_val) 
    {
        // Counter used to auto-increment
        if (++increment_ctr >= GLOBALS_INCREMENT_MAX_VAL) 
        {
            // Move player, should respond to button press
            if (!player_dead && !game_over) 
            {
                player_move();
            }

            // Init fire bullet if statement
            if ((buttons_val == GLOBALS_BTN_3) && !(bullet_moving) && !game_over) 
            {
                bullet_moving = true; // ste the flag
                current_pos_bullet = (current_pos_player + GUN_OFFSET) - GLOBALS_NEW_LINE * HEIGHT_OF_TANK_FOR_BULLET; // get position
            }

            increment_ctr = 0;
        }
    }


    /* This is the die ctr, it wil lshow an explosion then erase the alien */
    if (start_die_ctr) // once an alien gets hit, start the counter
    {
        if ((++die_ctr >= DIE_CTR_MAX) && !game_over) // so we can show an explosion and then erase
        {
            alien_erase_dead(); // erase
            die_ctr = 0; // reset ctr
            start_die_ctr = false; // reset flag
        }
    }

    // The time will auto-increment if pressed for 1/2 second
    if (game_over) 
    {
        if ((++half_sec_ctr >= GLOBALS_HALF_SECOND) && !name_entered)
        {
            game_over_blink_cursor(false); // this is our blink cursor, blinks after half second
            half_sec_ctr = 0; // reset counter
        }
    }
}


// This is invoked each time there is a change in the button state (result of a push or a bounce).
void isr_buttons()
{
    generic_reset_counters();
    new_buttons_val = read_buttons(); // get the button value
    clear_button_interrupts(); // clear button interrupts
    
    if (debounced) 
    {
        if (game_over && !name_entered) 
            respond_to_press();
        else if (!game_over && buttons_val == GLOBALS_BTN_2)
        {
            adjust_volume();
        }

        debounced = false;
    }
}

// This is invoked each time there is a change in switch state
void isr_switches()
{
    printf("Reading switches!!!\n\r");
    generic_reset_counters();
    new_switches_val = read_switches(); // read the switches
    clear_switches_interrupts(); // clear the interrupts
}
     
// This is our main function, this is the function that executes the code
int main() 
{
    // Init 
    audio_file = open("/dev/audio", O_RDWR);
    init();
    srand(time(0));
        
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
            
        if (interrupt_value & GLOBALS_FIT_MASK) // if fit is enabled go into isr_fit
        {
            isr_fit();
        }

        if (interrupt_value & GLOBALS_BTN_MASK) // if button mask detected an interrupt, go into usr_buttons()
        {
            isr_buttons();
        }

        if (interrupt_value & GLOBALS_SW_MASK) // if the switch detects an interrupt, go into isr_switches
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
    hdmi_close(); // exit hdmi
}
