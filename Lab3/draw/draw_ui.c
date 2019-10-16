#include "draw_ui.h"
#include "../sprites/sprites.c"
#include <time.h>

#define MAX_LIVES 5

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

void draw_ui_init() {
    srand(time(0));
}

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

void draw_ui_increase_score(uint16_t row) {
    switch (row) {
        case TOP_ALIEN:
            score += TOP_ALIEN_SCORE;
            break;
        case MID_ALIEN_0:
        case MID_ALIEN_1:
            score += MID_ALIEN_SCORE;
            break;
        case BOT_ALIEN_0:
        case BOT_ALIEN_1:
            score += BOT_ALIEN_SCORE;
            break;
    }
}

void draw_ui_increase_score_saucer() {
    // Get a random multiple of 10 between 50 and 250;
    score += (rand() % 25 + 5) * 10;
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

void draw_ui_increment_lives() {
    if (lives + 1 <= MAX_LIVES) {
        lives++;
        draw_ui_update_lives(false);
    }
}

void draw_ui_decrement_lives() {
    lives--;
    draw_ui_update_lives(false);
}

void draw_ui_update_lives(bool firstRun) 
{
    uint32_t pos = RIGHT_EDGE - LETTER_WIDTH * 9 + NEW_LINE * SIZE_SCALAR;

    for (uint8_t l = 0; l < MAX_LIVES; l++) {
        draw_alien(tank_15x8, pos, 15, 8, PIXEL_SIZE_GLOBAL, black);
        pos += 15 * PIXEL_SIZE_GLOBAL + 15;
    }

    for (uint8_t l = 0; l < lives; l++) {
        draw_alien(tank_15x8, pos, 15, 8, PIXEL_SIZE_GLOBAL, cyan);
        pos += 15 * PIXEL_SIZE_GLOBAL + 15;
    }
}