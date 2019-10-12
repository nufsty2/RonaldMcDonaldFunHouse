/* All includes that will be whatever calls this */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../hdmi/hdmi.h"
#include "../scores/scores.h"
#include "../sprites/sprites.c"
#include "../../Lab2/intc/intc.h"
#include "../../Lab2/buttons/buttons.h"
#include "../../Lab2/switches/switches.h"
#include "../draw/draw_generic.h"
#include "../draw/draw_alien.h"
#include "../draw/draw_ui.h"
#include "../draw/draw_game_over.h"

 /* Scalar to multiply size of pixel */
#define PIXEL_SIZE_GLOBAL 3
#define SIZE_SCALAR 2

/* Scalar for the width of the letter so it ain't small */
#define LETTER_WIDTH 39

/* How many digits in the score */ 
#define SCORE_DIGITS 5

/* Which interrupts are enabled masks */
#define FIT_MASK 0x1 
#define BTN_MASK 0x2 
#define SW_MASK  0x4 

/* Useful bitmasks for the switches and buttons */
#define BTN_0 0x1
#define BTN_1 0x2
#define BTN_2 0x4
#define BTN_3 0x8
#define SW_0  0x1

/* Max Values */
#define DEBOUNCE_MAX_VAL   5
#define SAUCER_MAX_VAL     3000
#define ALIEN_MOVE_MAX_VAL 10
#define INCREMENT_MAX_VAL  2

/* Defines for the FIT */
#define HALF_SECOND        50
#define FIT_1_SEC          100 // 10ms * 100  1000ms = 1s

/* Attributes for the screen and adjustments */
#define SCREEN_WIDTH 640
#define WHOLE_SCREEN_IN_BYTES 921600
#define RIGHT_EDGE (NEW_LINE - PIXEL_SIZE_GLOBAL)
#define NEW_LINE (SCREEN_WIDTH * PIXEL_SIZE_GLOBAL)
#define SPACE_BW_ALIENS 5

/* Starting pos for elements on the screen */
#define SAUCER_STARTING_POS NEW_LINE*40
#define ALIEN_START_POS NEW_LINE*50
#define PLAYER_START_POS NEW_LINE*460
#define BULLET_STARTING_POS (PLAYER_START_POS + 42) - NEW_LINE * 10 // 42 = at tank gun and subratct to move up

/* Aliens defines */
#define NO_ALIEN_X 11 // number of aliens width
#define NO_ALIEN_Y  5 // number of aliens height

/* Player defines */
#define NO_LIVES 3

/* Game over Defines */
#define DEFAULT_GAME_OVER_CHAR_0 'A'
#define DEFAULT_GAME_OVER_CHAR_1 'B'
#define DEFAULT_GAME_OVER_CHAR_2 'C'

/* Array width defines */
#define BLACK_SCALAR_POS 13
#define LINE_OF_BLACK_PIXELS (BLACK_SCALAR_POS + SPACE_BW_ALIENS) * SIZE_SCALAR * PIXEL_SIZE_GLOBAL * NO_ALIEN_X

/* Pixel colors constants */
static char   white[PIXEL_SIZE_GLOBAL]  = {0xFF, 0xFF, 0xFF};
static char   green[PIXEL_SIZE_GLOBAL]  = {0x00, 0xFF, 0x77};
static char   black[PIXEL_SIZE_GLOBAL]  = {0x00, 0x00, 0x00};
static char    cyan[PIXEL_SIZE_GLOBAL]  = {0x11, 0xFF, 0xFF};
static char     tan[PIXEL_SIZE_GLOBAL]  = {0xFF, 0x55, 0x55};
static char magenta[PIXEL_SIZE_GLOBAL]  = {0xFF, 0x66, 0xFF};
static char black_pixels[LINE_OF_BLACK_PIXELS];

/* All counters */
// button/switch debounce counter
static uint32_t debounce_ctr     = 0; // debounce counter
static uint32_t buttons_val      = 0; // buttons value
static uint32_t new_buttons_val  = 0; // new buttons value
// switch counter
static uint32_t switches_val     = 0; // switches value
static uint32_t new_switches_val = 0; // new switches value
// fit counters
static uint32_t fit_ctr       =  0;
static uint32_t half_sec_ctr  =  0;
static uint32_t increment_ctr =  0;
// alien move counters
static uint32_t alien_move_ctr = 0;
static uint32_t saucer_ctr = 0;
// bullet counter
static uint32_t bullet_ctr = 0;

/* Flags */
static bool debounced = false; // Flag used to determine if debounce timer is done
static bool blink = true;
static bool game_over = false;
static bool name_entered = false;

/* Our whole alien army array */
static const uint32_t* alien_army_sprites[5][11] = 
{
    {alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8   },
    {alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8},
    {alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8},
    {alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8},
    {alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8}
};

/* bool array to see which one is still alive */
static bool alien_army_is_alive[5][11] = 
{
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true}
};

/* Bool array to toggle to in/out sprites of the alien */
static bool alien_army_is_in[5][11] = 
{
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true}
};

/* Score digit variables */
static uint32_t score = 0;
static char score_digit_0 = 0;
static char score_digit_1 = 0;
static char score_digit_2 = 0;
static char score_digit_3 = 0;
static char score_digit_4 = 0;

/* Chars on the game over screen */
static char char_0 = DEFAULT_GAME_OVER_CHAR_0;
static char char_1 = DEFAULT_GAME_OVER_CHAR_1;
static char char_2 = DEFAULT_GAME_OVER_CHAR_2;
static uint8_t selected_char = 0;

/* Moving Alien attribtes */
static uint32_t current_pos_alien = ALIEN_START_POS;
static bool moving_right_alien = false;

/* Player attribtes */
static uint8_t lives = NO_LIVES;
static uint32_t current_pos_player = PLAYER_START_POS;
static bool moving_right_player = false;
static uint32_t current_pos_bullet = BULLET_STARTING_POS;

/* Saucer attributes */
static uint32_t saucer_pos = SAUCER_STARTING_POS;
static bool saucer_moving = false;

static bool bullet_moving = false;