#include "globals.h"
#include "../sprites/sprites.c"

/* All includes that will be whatever calls this */
// #include <stdio.h>
// #include <stdint.h>
// #include <stdbool.h>
// #include "../hdmi/hdmi.h"
// #include "../scores/scores.h"
// #include "../sprites/sprites.c"
// #include "../../Lab2/intc/intc.h"
// #include "../../Lab2/buttons/buttons.h"
// #include "../../Lab2/switches/switches.h"
// #include "../draw/draw_generic.h"
// #include "../draw/draw_alien.h"
// #include "../draw/draw_ui.h"
// #include "../draw/draw_game_over.h"
// #include "../draw/draw_player.h"

/* Pixel colors constants */
char   white[PIXEL_SIZE_GLOBAL]  = {0xFF, 0xFF, 0xFF};
char   green[PIXEL_SIZE_GLOBAL]  = {0x00, 0xFF, 0x77};
char   black[PIXEL_SIZE_GLOBAL]  = {0x00, 0x00, 0x00};
char    cyan[PIXEL_SIZE_GLOBAL]  = {0x11, 0xFF, 0xFF};
char     tan[PIXEL_SIZE_GLOBAL]  = {0xFF, 0x55, 0x55};
char magenta[PIXEL_SIZE_GLOBAL]  = {0xFF, 0x66, 0xFF};
char black_pixels[LINE_OF_BLACK_PIXELS];

/* All counters */
// button/switch debounce counter
uint32_t debounce_ctr     = 0; // debounce counter
uint32_t buttons_val      = 0; // buttons value
uint32_t new_buttons_val  = 0; // new buttons value
// switch counter
uint32_t switches_val     = 0; // switches value
uint32_t new_switches_val = 0; // new switches value
// fit counters
uint32_t fit_ctr       =  0;
uint32_t half_sec_ctr  =  0;
uint32_t increment_ctr =  0;
// alien move counters
uint32_t alien_move_ctr = 0;
uint32_t saucer_ctr = 0;
uint32_t die_ctr = 0;
bool start_die_ctr = false; 

/* Flags */
bool debounced = false; // Flag used to determine if debounce timer is done
bool blink = true;
bool game_over = false;
bool name_entered = false;

/* Chars on the game over screen */
char char_0 = DEFAULT_GAME_OVER_CHAR_0;
char char_1 = DEFAULT_GAME_OVER_CHAR_1;
char char_2 = DEFAULT_GAME_OVER_CHAR_2;
uint8_t selected_char = 0;

/* Moving Alien attribtes */
uint32_t current_pos_alien = ALIEN_START_POS;
bool moving_right_alien = false;

/* Player attribtes */
uint8_t lives = NO_LIVES;
uint32_t current_pos_player = PLAYER_START_POS;

/* Saucer attributes */
uint32_t current_pos_saucer = SAUCER_STARTING_POS;
bool saucer_moving = false;

/* Player bullet attributes */
bool bullet_moving = false;
uint32_t current_pos_bullet = BULLET_STARTING_POS;

/* Alien bullet attributes */
bool alien_bullets_moving[4] = { false, false, false, false };
uint32_t current_alien_pos_bullets[4] = { 0, 0, 0, 0 }; // the starting pos is a random number

/* Eroding Bunker Attributes */
uint32_t bunker_pos[4] = { 0, 0, 0, 0 };

