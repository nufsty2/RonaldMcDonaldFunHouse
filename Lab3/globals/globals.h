#ifndef GLOABLS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/* Scalar to multiply size of pixel */
#define PIXEL_SIZE_GLOBAL 3
#define SIZE_SCALAR 2

/* Which pixel to write on */
#define PIXEL_BYTE_0 0
#define PIXEL_BYTE_1 1
#define PIXEL_BYTE_2 2

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
#define DEBOUNCE_MAX_VAL        5
#define SAUCER_MAX_VAL          1000
#define ALIEN_MOVE_MAX_VAL      10
#define INCREMENT_MAX_VAL       2
#define ALIEN_BULLET_MAX_VAL    600
#define ALIEN_BULLET_MIN_VAL    100
#define PLAYER_DEATH_STAGE_1    25
#define PLAYER_DEATH_STAGE_2    50
#define PLAYER_DEATH_STAGE_3    75
#define PLAYER_DEATH_MAX_VAL    100
#define MAX_BULLETS 4

/* Defines for the FIT */
#define HALF_SECOND        50
#define FIT_1_SEC          100 // 10ms * 100  1000ms = 1s

/* Attributes for the screen and adjustments */
#define SCREEN_WIDTH 640
#define WHOLE_SCREEN_IN_BYTES 921600
#define RIGHT_EDGE (NEW_LINE - PIXEL_SIZE_GLOBAL)
#define NEW_LINE (SCREEN_WIDTH * PIXEL_SIZE_GLOBAL)
#define SPACE_BW_ALIENS 5
#define BLACK_PIXEL 0x00

/* Starting pos for elements on the screen */
#define SAUCER_STARTING_POS NEW_LINE*40
#define ALIEN_START_POS NEW_LINE*50
#define PLAYER_START_POS NEW_LINE*460
#define BULLET_STARTING_POS (PLAYER_START_POS + 42) - NEW_LINE * 10 // 42 = at tank gun and subratct to move up

/* Aliens defines */
#define NO_ALIEN_X 11 // number of aliens width
#define NO_ALIEN_Y  5 // number of aliens height
#define SPACE_MOVING_ALIENS 13
#define MOVE_ROWS_DOWN_FOR_ALIENS 15
#define ALIEN_SPRITE_WIDTH 13
#define ALIEN_SPRITE_HEIGHT 8
#define FAR_RIGHT_BOUNDRY_FOR_SAUCER 1818
#define SAUCER_WIDTH 16
#define SAUCER_HEIGHT 7

/* Black block attributes */
#define BLOCK_WIDTH 2
#define BLOCK_HEIGHT 8

/* Player defines */
#define NO_LIVES 3
#define TANK_WIDTH 15
#define TANK_HEIGHT 8

/* Game over Defines */
#define DEFAULT_GAME_OVER_CHAR_0 'A'
#define DEFAULT_GAME_OVER_CHAR_1 'B'
#define DEFAULT_GAME_OVER_CHAR_2 'C'
#define GAME_OVER_LETTERS_SCALAR 640*40
#define LETTER_DIM 5
#define NO_SCORES_TO_DISPLAY 10

/* Array width defines */
#define BLACK_SCALAR_POS 13
#define LINE_OF_BLACK_PIXELS (BLACK_SCALAR_POS + SPACE_BW_ALIENS) * SIZE_SCALAR * PIXEL_SIZE_GLOBAL * NO_ALIEN_X

/* Game over defines */
#define LETTER_0_SCORE 0
#define LETTER_1_SCORE 1
#define LETTER_2_SCORE 2
#define SCORE_DIGET_0 0
#define SCORE_DIGET_1 1
#define SCORE_DIGET_2 2
#define SCORE_DIGET_3 3
#define SCORE_DIGET_4 4

/* Alien army */
#define TOP_ALIEN 0
#define MID_ALIEN_0 1
#define MID_ALIEN_1 2
#define BOT_ALIEN_0 3
#define BOT_ALIEN_1 4
#define TOP_ALIEN_SCORE 40
#define MID_ALIEN_SCORE 20
#define BOT_ALIEN_SCORE 10 


#endif