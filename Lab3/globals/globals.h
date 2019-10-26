#ifndef GLOABLS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/* Scalar to multiply size of pixel */
#define GLOBALS_PIXEL_SIZE 3
#define GLOBALS_SIZE_SCALAR 2

/* Which pixel to write on */
#define GLOBALS_PIXEL_BYTE_0 0
#define GLOBALS_PIXEL_BYTE_1 1
#define GLOBALS_PIXEL_BYTE_2 2

/* Scalar for the width of the letter so it ain't small */
#define GLOBALS_LETTER_WIDTH 39

/* How many digits in the score */ 
#define GLOBALS_SCORE_DIGITS 5

/* Which interrupts are enabled masks */
#define GLOBALS_FIT_MASK 0x1 
#define GLOBALS_BTN_MASK 0x2 
#define GLOBALS_SW_MASK  0x4 

/* Useful bitmasks for the switches and buttons */
#define GLOBALS_BTN_0 0x1
#define GLOBALS_BTN_1 0x2
#define GLOBALS_BTN_2 0x4
#define GLOBALS_BTN_3 0x8
#define GLOBALS_SW_0  0x1

/* Max Values */
#define GLOBALS_DEBOUNCE_MAX_VAL        5
#define GLOBALS_SAUCER_MAX_VAL          1000
#define GLOBALS_ALIEN_MOVE_MAX_VAL      10
#define GLOBALS_INCREMENT_MAX_VAL       2
#define GLOBALS_ALIEN_BULLET_MAX_VAL    30
#define GLOBALS_ALIEN_BULLET_MIN_VAL    1
#define GLOBALS_PLAYER_DEATH_STAGE_1    25
#define GLOBALS_PLAYER_DEATH_STAGE_2    50
#define GLOBALS_PLAYER_DEATH_STAGE_3    75
#define GLOBALS_PLAYER_DEATH_MAX_VAL    100
#define GLOBALS_MAX_BULLETS 4

/* Defines for the FIT */
#define GLOBALS_HALF_SECOND        50
#define GLOBALS_FIT_1_SEC          100 // 10ms * 100  1000ms = 1s

/* Attributes for the screen and adjustments */
#define GLOBALS_SCREEN_WIDTH 640
#define GLOBALS_WHOLE_SCREEN_IN_BYTES 921600
#define GLOBALS_RIGHT_EDGE (GLOBALS_NEW_LINE - GLOBALS_PIXEL_SIZE)
#define GLOBALS_NEW_LINE (GLOBALS_SCREEN_WIDTH * GLOBALS_PIXEL_SIZE)
#define GLOBALS_SPACE_BW_ALIENS 5
#define GLOBALS_BLACK_PIXEL 0x00

/* Starting pos for elements on the screen */
#define GLOBALS_SAUCER_STARTING_POS GLOBALS_NEW_LINE*40
#define GLOBALS_ALIEN_START_POS GLOBALS_NEW_LINE*50
#define GLOBALS_PLAYER_START_POS GLOBALS_NEW_LINE*460
#define GLOBALS_BULLET_STARTING_POS (GLOBALS_PLAYER_START_POS + 42) - GLOBALS_NEW_LINE * 10 // 42 = at tank gun and subratct to move up

/* Aliens defines */
#define GLOBALS_NO_ALIEN_X 11 // number of aliens width
#define GLOBALS_NO_ALIEN_Y  5 // number of aliens height
#define GLOBALS_SPACE_MOVING_ALIENS 13
#define GLOBALS_MOVE_ROWS_DOWN_FOR_ALIENS 15
#define GLOBALS_ALIEN_SPRITE_WIDTH 13
#define GLOBALS_ALIEN_SPRITE_HEIGHT 8
#define GLOBALS_FAR_RIGHT_BOUNDRY_FOR_SAUCER 1818
#define GLOBALS_SAUCER_WIDTH 16
#define GLOBALS_SAUCER_HEIGHT 7

/* Alien bullet defines */
#define GLOBALS_ALIEN_BULLET_WIDTH 3
#define GLOBALS_ALIEN_BULLET_HEIGHT 5

/* Black block attributes */
#define GLOBALS_BLOCK_WIDTH 2
#define GLOBALS_BLOCK_HEIGHT 8

/* Player defines */
#define GLOBALS_NO_LIVES 3
#define GLOBALS_TANK_WIDTH 15
#define GLOBALS_TANK_HEIGHT 8

/* Player bullet defines */
#define GLOBALS_PLAYER_BULLET_WIDTH 1
#define GLOBALS_PLAYER_BULLET_HEIGHT 5

/* Game over Defines */
#define GLOBALS_DEFAULT_GAME_OVER_CHAR_0 'A'
#define GLOBALS_DEFAULT_GAME_OVER_CHAR_1 'B'
#define GLOBALS_DEFAULT_GAME_OVER_CHAR_2 'C'
#define GLOBALS_GAME_OVER_LETTERS_SCALAR 640*40
#define GLOBALS_LETTER_DIM 5
#define GLOBALS_NO_SCORES_TO_DISPLAY 10

/* Array width defines */
#define GLOBALS_BLACK_SCALAR_POS 13
#define GLOBALS_LINE_OF_BLACK_PIXELS (GLOBALS_BLACK_SCALAR_POS + GLOBALS_SPACE_BW_ALIENS) * GLOBALS_SIZE_SCALAR * GLOBALS_PIXEL_SIZE * GLOBALS_NO_ALIEN_X

/* Game over defines */
#define GLOBALS_LETTER_0_SCORE 0
#define GLOBALS_LETTER_1_SCORE 1
#define GLOBALS_LETTER_2_SCORE 2
#define GLOBALS_SCORE_DIGIT_0 0
#define GLOBALS_SCORE_DIGIT_1 1
#define GLOBALS_SCORE_DIGIT_2 2
#define GLOBALS_SCORE_DIGIT_3 3
#define GLOBALS_SCORE_DIGIT_4 4
#define GLOBALS_VALUE_CHAR_0 4
#define GLOBALS_VALUE_CHAR_1 5
#define GLOBALS_VALUE_CHAR_2 6
#define GLOBALS_VALUE_CHAR_3 7
#define GLOBALS_VALUE_CHAR_4 8


/* Alien army */
#define GLOBALS_TOP_ALIEN 0
#define GLOBALS_MID_ALIEN_0 1
#define GLOBALS_MID_ALIEN_1 2
#define GLOBALS_BOT_ALIEN_0 3
#define GLOBALS_BOT_ALIEN_1 4
#define GLOBALS_TOP_ALIEN_SCORE 40
#define GLOBALS_MID_ALIEN_SCORE 20
#define GLOBALS_BOT_ALIEN_SCORE 10 

/* Black box defines */
#define GLOBALS_BLACK_BOX_DIM 6 


#endif