#include "globals.h"
#include "../sprites/sprites.c"

/* Pixel colors constants */
char   white[GLOBALS_PIXEL_SIZE]  = {0xFF, 0xFF, 0xFF};
char   green[GLOBALS_PIXEL_SIZE]  = {0x00, 0xFF, 0x77};
char   black[GLOBALS_PIXEL_SIZE]  = {0x00, 0x00, 0x00};
char    cyan[GLOBALS_PIXEL_SIZE]  = {0x11, 0xFF, 0xFF};
char     red[GLOBALS_PIXEL_SIZE]  = {0xFF, 0x55, 0x55};
char magenta[GLOBALS_PIXEL_SIZE]  = {0xFF, 0x66, 0xFF};
char black_pixels[GLOBALS_LINE_OF_BLACK_PIXELS];

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
bool player_dead = false;

/* Chars on the game over screen */
char char_0 = GLOBALS_DEFAULT_GAME_OVER_CHAR_0;
char char_1 = GLOBALS_DEFAULT_GAME_OVER_CHAR_1;
char char_2 = GLOBALS_DEFAULT_GAME_OVER_CHAR_2;
char selected_char = 0;

/* Moving Alien attribtes */
uint32_t current_pos_alien = GLOBALS_ALIEN_START_POS;
bool moving_right_alien = false;

/* Player attribtes */
uint8_t lives = GLOBALS_NO_LIVES;
uint32_t current_pos_player = GLOBALS_PLAYER_START_POS;
bool new_shot = true;

/* Saucer attributes */
uint32_t current_pos_saucer = GLOBALS_SAUCER_STARTING_POS;
bool saucer_moving = false;

/* Player bullet attributes */
bool bullet_moving = false;
uint32_t current_pos_bullet = GLOBALS_BULLET_STARTING_POS;

/* Alien bullet attributes */
uint32_t alien_bullet_pos[GLOBALS_MAX_BULLETS] = {0, 0, 0, 0};
bool alien_bullet_moving[GLOBALS_MAX_BULLETS] = {false, false, false, false};