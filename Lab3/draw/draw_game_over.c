#include "../globals/globals.h"

void main_print_score(score_t score, uint32_t pos)
{
    draw_alien(get_sprite_from_digit(score.name[0]),       pos + LETTER_WIDTH*0, 5, 5, PIXEL_SIZE_GLOBAL*2, cyan);
    draw_alien(get_sprite_from_digit(score.name[1]),       pos + LETTER_WIDTH*1, 5, 5, PIXEL_SIZE_GLOBAL*2, cyan);
    draw_alien(get_sprite_from_digit(score.name[2]),       pos + LETTER_WIDTH*2, 5, 5, PIXEL_SIZE_GLOBAL*2, cyan);
    draw_alien(get_sprite_from_digit(score.value_char[0]), pos + LETTER_WIDTH*4, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(get_sprite_from_digit(score.value_char[1]), pos + LETTER_WIDTH*5, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(get_sprite_from_digit(score.value_char[2]), pos + LETTER_WIDTH*6, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(get_sprite_from_digit(score.value_char[3]), pos + LETTER_WIDTH*7, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(get_sprite_from_digit(score.value_char[4]), pos + LETTER_WIDTH*8, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
}

void main_print_scores()
{
    // ENTER
    draw_alien(letterE_5x5, LETTER_WIDTH*20 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(letterN_5x5, LETTER_WIDTH*21 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(letterT_5x5, LETTER_WIDTH*22 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(letterE_5x5, LETTER_WIDTH*23 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(letterR_5x5, LETTER_WIDTH*24 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    // NAME
    draw_alien(letterN_5x5, LETTER_WIDTH*26 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(letterA_5x5, LETTER_WIDTH*27 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(letterM_5x5, LETTER_WIDTH*28 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(letterE_5x5, LETTER_WIDTH*29 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    // AAA
    draw_alien(get_sprite_from_digit(char_0), LETTER_WIDTH*24 + 640*80*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(get_sprite_from_digit(char_1), LETTER_WIDTH*25 + 640*80*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(get_sprite_from_digit(char_2), LETTER_WIDTH*26 + 640*80*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);

    score_t top_ten[10];
    scores_get_top_10_scores(top_ten);
    for (uint8_t i = 0; i < 10; i++) 
    {
        main_print_score(top_ten[i], PIXEL_SIZE_GLOBAL*((80+20*i)*640) + LETTER_WIDTH*20);
    }
}

void blink_cursor(bool force) 
{
    draw_sprite(letterBLOCK_6x6, 
                LETTER_WIDTH*24 - 3 + (selected_char * LETTER_WIDTH) + 640*79*PIXEL_SIZE_GLOBAL, 
                6, 
                6, 
                PIXEL_SIZE_GLOBAL*2, 
                force ? black : 
                    blink ? cyan : black
    );
    draw_sprite(get_sprite_from_digit(*get_selected_char()),     
                LETTER_WIDTH*24 +     (selected_char * LETTER_WIDTH) + 640*80*PIXEL_SIZE_GLOBAL, 
                5, 
                5, 
                PIXEL_SIZE_GLOBAL*2, 
                force ? cyan : 
                    blink ? black : cyan
    );  
    blink = !blink;
}