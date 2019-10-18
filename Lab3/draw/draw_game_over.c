#include "draw_game_over.h"
#include "../scores/scores.h"
#include "draw_generic.h"


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
    for (uint8_t i = 0; i < 3; i++) 
    {
        if (score.name[i] == NULL) 
        {
            return;
        }
    }

    draw_alien(get_sprite_from_digit(score.name[LETTER_0_SCORE]),       pos + LETTER_WIDTH*0, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, cyan);
    draw_alien(get_sprite_from_digit(score.name[LETTER_1_SCORE]),       pos + LETTER_WIDTH*1, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, cyan);
    draw_alien(get_sprite_from_digit(score.name[LETTER_2_SCORE]),       pos + LETTER_WIDTH*2, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, cyan);
    draw_alien(get_sprite_from_digit(score.value_char[SCORE_DIGIT_0]),  pos + LETTER_WIDTH*4, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(get_sprite_from_digit(score.value_char[SCORE_DIGIT_1]),  pos + LETTER_WIDTH*5, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(get_sprite_from_digit(score.value_char[SCORE_DIGIT_2]),  pos + LETTER_WIDTH*6, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(get_sprite_from_digit(score.value_char[SCORE_DIGIT_3]),  pos + LETTER_WIDTH*7, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(get_sprite_from_digit(score.value_char[SCORE_DIGIT_4]),  pos + LETTER_WIDTH*8, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
}

// This is our main function print all the scores
void main_print_scores()
{
    // ENTER
    draw_alien(letterE_5x5, LETTER_WIDTH*20 + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(letterN_5x5, LETTER_WIDTH*21 + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(letterT_5x5, LETTER_WIDTH*22 + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(letterE_5x5, LETTER_WIDTH*23 + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(letterR_5x5, LETTER_WIDTH*24 + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    // NAME
    draw_alien(letterN_5x5, LETTER_WIDTH*26 + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(letterA_5x5, LETTER_WIDTH*27 + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(letterM_5x5, LETTER_WIDTH*28 + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(letterE_5x5, LETTER_WIDTH*29 + GAME_OVER_LETTERS_SCALAR*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    // AAA
    draw_alien(get_sprite_from_digit(char_0), LETTER_WIDTH*24 + 640*80*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(get_sprite_from_digit(char_1), LETTER_WIDTH*25 + 640*80*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
    draw_alien(get_sprite_from_digit(char_2), LETTER_WIDTH*26 + 640*80*PIXEL_SIZE_GLOBAL, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);

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