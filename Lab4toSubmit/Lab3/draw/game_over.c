#include "game_over.h"
#include "../sprites/sprites.c"
#include "generic.h"
#include "ui.h"

/* Defines in this C file */
#define NUM_CHARS_IN_SCORE 3
#define PRINT_SCORE_0 0
#define PRINT_SCORE_1 1
#define PRINT_SCORE_2 2
#define PRINT_SCORE_3 3
#define PRINT_SCORE_4 4
#define PRINT_SCORE_5 5
#define PRINT_SCORE_6 6
#define PRINT_SCORE_7 7
#define PRINT_SCORE_8 8

/* Defines for ENTER NAME */
#define OFFSET_E_0 20
#define OFFEST_N_0 21
#define OFFEST_T_0 22
#define OFFEST_E_1 23
#define OFFEST_R_0 24
#define OFFEST_N_1 26
#define OFFEST_A_0 27
#define OFFEST_M_0 28
#define OFFEST_E_2 29

/* Defines for entering the name (AAA) */
#define OFFSET_CHAR_0 24
#define OFFSET_CHAR_1 25
#define OFFSET_CHAR_2 26
#define OFFSET_ROW_FOR_CHARS 640*80

#define BLINK_OFFSET_1  79
#define BLINK_OFFSET_2  80 
#define LETTER_WIDTH_SCALAR 24
#define BASE_OFFSET 80
#define SCORE_PADDING 20
#define CURSOR_DIM 5


/* Flags */
extern bool blink;

/* Score attributes*/ 
extern uint32_t score;
extern char char_0;
extern char char_1;
extern char char_2;
extern char selected_char;

/* Colors */
extern char black[];
extern char green[];
extern char cyan[];
extern char white[];

// This is our main function to print main scores
// @param score - the score to pass in
// @param pos - the position of the number
void game_over_print_score(score_t score, uint32_t pos)
{
    // Check for errors first
    for (uint8_t i = 0; i < NUM_CHARS_IN_SCORE; i++) 
    {
        if (score.name[i] < 'A') // if error
        {
            return;
        }
    }

    generic_draw_sprite(generic_get_sprite_from_digit(score.name[GLOBALS_LETTER_0_SCORE]),       pos + GLOBALS_LETTER_WIDTH*PRINT_SCORE_0, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, cyan);
    generic_draw_sprite(generic_get_sprite_from_digit(score.name[GLOBALS_LETTER_1_SCORE]),       pos + GLOBALS_LETTER_WIDTH*PRINT_SCORE_1, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, cyan);
    generic_draw_sprite(generic_get_sprite_from_digit(score.name[GLOBALS_LETTER_2_SCORE]),       pos + GLOBALS_LETTER_WIDTH*PRINT_SCORE_2, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, cyan);
    generic_draw_sprite(generic_get_sprite_from_digit(score.value_char[GLOBALS_SCORE_DIGIT_0]),  pos + GLOBALS_LETTER_WIDTH*PRINT_SCORE_4, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    generic_draw_sprite(generic_get_sprite_from_digit(score.value_char[GLOBALS_SCORE_DIGIT_1]),  pos + GLOBALS_LETTER_WIDTH*PRINT_SCORE_5, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    generic_draw_sprite(generic_get_sprite_from_digit(score.value_char[GLOBALS_SCORE_DIGIT_2]),  pos + GLOBALS_LETTER_WIDTH*PRINT_SCORE_6, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    generic_draw_sprite(generic_get_sprite_from_digit(score.value_char[GLOBALS_SCORE_DIGIT_3]),  pos + GLOBALS_LETTER_WIDTH*PRINT_SCORE_7, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    generic_draw_sprite(generic_get_sprite_from_digit(score.value_char[GLOBALS_SCORE_DIGIT_4]),  pos + GLOBALS_LETTER_WIDTH*PRINT_SCORE_8, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
}

// This is our main function print all the scores
void game_over_print_scores()
{
    // Erase ENTER
    generic_draw_sprite(letterE_5x5, GLOBALS_LETTER_WIDTH*OFFSET_E_0 + GLOBALS_GAME_OVER_LETTERS_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black);
    generic_draw_sprite(letterN_5x5, GLOBALS_LETTER_WIDTH*OFFEST_N_0 + GLOBALS_GAME_OVER_LETTERS_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black);
    generic_draw_sprite(letterT_5x5, GLOBALS_LETTER_WIDTH*OFFEST_T_0 + GLOBALS_GAME_OVER_LETTERS_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black);
    generic_draw_sprite(letterE_5x5, GLOBALS_LETTER_WIDTH*OFFEST_E_1 + GLOBALS_GAME_OVER_LETTERS_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black);
    generic_draw_sprite(letterR_5x5, GLOBALS_LETTER_WIDTH*OFFEST_R_0 + GLOBALS_GAME_OVER_LETTERS_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black);
    // Erase NAME
    generic_draw_sprite(letterN_5x5, GLOBALS_LETTER_WIDTH*OFFEST_N_1 + GLOBALS_GAME_OVER_LETTERS_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black);
    generic_draw_sprite(letterA_5x5, GLOBALS_LETTER_WIDTH*OFFEST_A_0 + GLOBALS_GAME_OVER_LETTERS_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black);
    generic_draw_sprite(letterM_5x5, GLOBALS_LETTER_WIDTH*OFFEST_M_0 + GLOBALS_GAME_OVER_LETTERS_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black);
    generic_draw_sprite(letterE_5x5, GLOBALS_LETTER_WIDTH*OFFEST_E_2 + GLOBALS_GAME_OVER_LETTERS_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black);
    // Draw initials
    generic_draw_sprite(generic_get_sprite_from_digit(char_0), GLOBALS_LETTER_WIDTH*OFFSET_CHAR_0 + OFFSET_ROW_FOR_CHARS*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black);
    generic_draw_sprite(generic_get_sprite_from_digit(char_1), GLOBALS_LETTER_WIDTH*OFFSET_CHAR_1 + OFFSET_ROW_FOR_CHARS*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black);
    generic_draw_sprite(generic_get_sprite_from_digit(char_2), GLOBALS_LETTER_WIDTH*OFFSET_CHAR_2 + OFFSET_ROW_FOR_CHARS*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black);

    // get the top ten
    score_t top_ten[GLOBALS_NO_SCORES_TO_DISPLAY];
    scores_get_top_10_scores(top_ten);
    // For loop to print top 10
    for (uint8_t i = 0; i < GLOBALS_NO_SCORES_TO_DISPLAY; i++) 
    {
        game_over_print_score(top_ten[i], GLOBALS_NEW_LINE*((BASE_OFFSET+SCORE_PADDING*i)) + GLOBALS_LETTER_WIDTH*SCORE_PADDING);
    }
}

// This is our blink cursor function to blink the selected cursor
// @param force - forces the color change of the letter
void game_over_blink_cursor(bool force) 
{
    // Draw the sprite
    generic_draw_sprite(letterBLOCK_6x6, 
                GLOBALS_LETTER_WIDTH*LETTER_WIDTH_SCALAR - GLOBALS_PIXEL_SIZE + (selected_char * GLOBALS_LETTER_WIDTH) + BLINK_OFFSET_1*GLOBALS_NEW_LINE, 
                GLOBALS_BLACK_BOX_DIM, 
                GLOBALS_BLACK_BOX_DIM, 
                GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, 
                force ? black : 
                    blink ? cyan : black
    );
    char* current = ui_get_selected_char(); // get the current selected cursors
    generic_draw_sprite(generic_get_sprite_from_digit(*current),     
                GLOBALS_LETTER_WIDTH*LETTER_WIDTH_SCALAR +     (selected_char * GLOBALS_LETTER_WIDTH) + BLINK_OFFSET_2*GLOBALS_NEW_LINE, 
                CURSOR_DIM, 
                CURSOR_DIM, 
                GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, 
                force ? cyan : 
                    blink ? black : cyan
    );  
    blink = !blink; // set blink flags
}