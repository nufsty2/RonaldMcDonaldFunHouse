#include "draw_ui.h"

extern uint32_t score;
extern uint8_t lives;
extern char black[];
extern char green[];
extern char cyan[];
extern char selected_char;
extern char char_0;
extern char char_1;
extern char char_2;

/* Score digit variables */
uint32_t score = 0;
char score_digit_0 = 0;
char score_digit_1 = 0;
char score_digit_2 = 0;
char score_digit_3 = 0;
char score_digit_4 = 0;


char* get_score_digit(uint8_t digit) 
{
    switch (digit) {
        case 0:
            return &score_digit_0;
        case 1:
            return &score_digit_1;
        case 2:
            return &score_digit_2;
        case 3:
            return &score_digit_3;
        case 4:
            return &score_digit_4;
        default:
            return NULL;
    }
}

char* get_selected_char() 
{
    switch(selected_char) 
    {
        case 0:
            return &char_0;
        case 1:
            return &char_1;
        case 2:
            return &char_2;
        default:
            return NULL;
    }
}

void update_score(bool firstRun) 
{
    uint32_t score_copy = score;
    uint32_t digit_location = LETTER_WIDTH * 10;

    // Print numbers
    for (uint16_t i = 0; i < SCORE_DIGITS; i++)
    {
        uint8_t digit_val = score_copy % 10;

        if ((*get_score_digit(i) != digit_val) && !firstRun)
        {
            // Erase
            draw_sprite(get_sprite_from_digit(*get_score_digit(i)), digit_location, 5, 5, PIXEL_SIZE_GLOBAL * 2, black);
            // Draw
            draw_sprite(get_sprite_from_digit(digit_val), digit_location, 5, 5, PIXEL_SIZE_GLOBAL * 2, green);
            // Replace value
            *get_score_digit(i) = digit_val;
        }
        else if (firstRun)
        {
            // Draw
            draw_alien(get_sprite_from_digit(digit_val), digit_location, 5, 5, PIXEL_SIZE_GLOBAL * 2, green);
        }

        score_copy /= 10;
        digit_location -= LETTER_WIDTH;
    }
}

void update_lives(bool firstRun) 
{
    uint32_t pos = RIGHT_EDGE - LETTER_WIDTH * 9 + 640 * 3 * 2;
    for (uint8_t l = 0; l < lives; l++) {
        draw_alien(tank_15x8, pos, 15, 8, PIXEL_SIZE_GLOBAL, cyan);
        pos += 15 * PIXEL_SIZE_GLOBAL + 15;
    }
}