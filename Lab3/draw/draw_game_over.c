#include "draw_game_over.h"
#include "../scores/scores.h"
#include "draw_generic.h"

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
#define OFFSET_E 20
#define OFFEST_N 21
#define OFFEST_T 22
#define OFFEST_E 23
#define OFFEST_R 24
 ///
#define OFFEST_N 26
#define OFFEST_A 27
#define OFFEST_M 28
#define OFFEST_E 29

/* Defines for entering the name (AAA) */
#define OFFSET_CHAR_0 24
#define OFFSET_CHAR_1 25
#define OFFSET_CHAR_2 26
#define OFFSET_ROW_FOR_CHARS 640*80


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
void main_print_score(score_t score, uint32_t pos)
{
    // Check for errors first
    for (uint8_t i = 0; i < NUM_CHARS_IN_SCORE; i++) 
    {
        if (score.name[i] == NULL) 
        {
            return;
        }
    }

    draw_alien(get_sprite_from_digit(score.name[LETTER_0_SCORE]),       pos + LETTER_WIDTH*PRINT_SCORE_0, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, cyan);
    draw_alien(get_sprite_from_digit(score.name[LETTER_1_SCORE]),       pos + LETTER_WIDTH*PRINT_SCORE_1, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, cyan);
    draw_alien(get_sprite_from_digit(score.name[LETTER_2_SCORE]),       pos + LETTER_WIDTH*PRINT_SCORE_2, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, cyan);
    draw_alien(get_sprite_from_digit(score.value_char[SCORE_DIGIT_0]),  pos + LETTER_WIDTH*PRINT_SCORE_4, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(get_sprite_from_digit(score.value_char[SCORE_DIGIT_1]),  pos + LETTER_WIDTH*PRINT_SCORE_5, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(get_sprite_from_digit(score.value_char[SCORE_DIGIT_2]),  pos + LETTER_WIDTH*PRINT_SCORE_6, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(get_sprite_from_digit(score.value_char[SCORE_DIGIT_3]),  pos + LETTER_WIDTH*PRINT_SCORE_7, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(get_sprite_from_digit(score.value_char[SCORE_DIGIT_4]),  pos + LETTER_WIDTH*PRINT_SCORE_8, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
}

// This is our main function print all the scores
void main_print_scores()
{
    // ENTER
    draw_alien(letterE_5x5, LETTER_WIDTH*OFFEST_E + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(letterN_5x5, LETTER_WIDTH*OFFEST_N + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(letterT_5x5, LETTER_WIDTH*OFFEST_T + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(letterE_5x5, LETTER_WIDTH*OFFEST_E + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(letterR_5x5, LETTER_WIDTH*OFFEST_R + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    // NAME
    draw_alien(letterN_5x5, LETTER_WIDTH*OFFEST_N + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(letterA_5x5, LETTER_WIDTH*OFFEST_A + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(letterM_5x5, LETTER_WIDTH*OFFEST_M + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(letterE_5x5, LETTER_WIDTH*OFFEST_E + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    // AAA
    draw_alien(get_sprite_from_digit(char_0), LETTER_WIDTH*OFFSET_CHAR_0 + OFFSET_ROW_FOR_CHARS*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(get_sprite_from_digit(char_1), LETTER_WIDTH*OFFSET_CHAR_1 + OFFSET_ROW_FOR_CHARS*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(get_sprite_from_digit(char_2), LETTER_WIDTH*OFFSET_CHAR_2 + OFFSET_ROW_FOR_CHARS*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);

    // get the top ten
    score_t top_ten[NO_SCORES_TO_DISPLAY];
    scores_get_top_10_scores(top_ten);
    // For loop to print top 10
    for (uint8_t i = 0; i < NO_SCORES_TO_DISPLAY; i++) 
    {
        main_print_score(top_ten[i], PIXEL_SIZE_GLOBAL*((80+20*i)*640) + LETTER_WIDTH*20);
    }
}

// This is our blink cursor function to blink the selected cursor
// @param force - forces the color change of the letter
void blink_cursor(bool force) 
{
    // Draw the sprite
    draw_sprite(letterBLOCK_6x6, 
                LETTER_WIDTH*24 - 3 + (selected_char * LETTER_WIDTH) + 640*79*PIXEL_SIZE_GLOBAL, 
                6, 
                6, 
                PIXEL_SIZE_GLOBAL*SIZE_SCALAR, 
                force ? black : 
                    blink ? cyan : black
    );
    char* current = get_selected_char();
    draw_sprite(get_sprite_from_digit(*current),     
                LETTER_WIDTH*24 +     (selected_char * LETTER_WIDTH) + 640*80*PIXEL_SIZE_GLOBAL, 
                5, 
                5, 
                PIXEL_SIZE_GLOBAL*SIZE_SCALAR, 
                force ? cyan : 
                    blink ? black : cyan
    );  
    blink = !blink; // set blink flags
}