#include "ui.h"
#include "../sprites/sprites.c"
#include "generic.h"
#include "alien.h"

// Defines only in this C file
#define MAX_LIVES 5
#define RANDOM_SCORE_SAUCER_START 30 + 5
#define RANDOM_SCORE_SAUCER_SCALAR 10
#define DIGIT_OFFSET 10
#define WIDTH_SCALAR 9

extern uint32_t score;
extern uint8_t lives;

/* Colors */
extern char black[];
extern char green[];
extern char cyan[];

/* Chars for the score screen */
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

// Randomize initializer
void ui_init() 
{
    srand(time(0));
}

// This function gets the score digit
// @param - digit - the digit we want to get
// @return - returns address of the score digit
char* ui_get_score_digit(uint8_t digit) 
{
    switch (digit) 
    {
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
            printf("Hit default in ui_get_score_digit()!\n\r"); // debug
            break;
    }
}

// This function gets the char
// @ return - address of the score char
char* ui_get_selected_char() 
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
            printf("Hit default in ui_get_selected_char()!\n\r");
            break;
    }
}

// This function increments the score
// @param row - whatever row it's on
void ui_increase_score(uint16_t row) 
{
    switch (row) 
    {
        case GLOBALS_TOP_ALIEN:
            score += GLOBALS_TOP_ALIEN_SCORE;
            break;
        case GLOBALS_MID_ALIEN_0:
        case GLOBALS_MID_ALIEN_1:
            score += GLOBALS_MID_ALIEN_SCORE;
            break;
        case GLOBALS_BOT_ALIEN_0:
        case GLOBALS_BOT_ALIEN_1:
            score += GLOBALS_BOT_ALIEN_SCORE;
            break;
    }
}

// This incs the score randomly for the saucer
void ui_increase_score_saucer() 
{
    // Get a random multiple of 10 between 50 and 250;
    score += (rand() % RANDOM_SCORE_SAUCER_START) * RANDOM_SCORE_SAUCER_SCALAR;
}

// This function updates the score
// @param firstRun - If first run, make it zero
void ui_update_score(bool firstRun) 
{
    uint32_t score_copy = score;
    uint32_t digit_location = GLOBALS_LETTER_WIDTH * DIGIT_OFFSET;

    // Print numbers
    for (uint16_t i = 0; i < GLOBALS_SCORE_DIGITS; i++)
    {
        uint8_t digit_val = score_copy % DIGIT_OFFSET;

        if ((*ui_get_score_digit(i) != digit_val) && !firstRun)
        {
            // Erase
            generic_draw_sprite(generic_get_sprite_from_digit(*ui_get_score_digit(i)), digit_location, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE * GLOBALS_SIZE_SCALAR, black);
            // Draw
            generic_draw_sprite(generic_get_sprite_from_digit(digit_val), digit_location, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE * GLOBALS_SIZE_SCALAR, green);
            // Replace value
            *ui_get_score_digit(i) = digit_val;
        }
        else if (firstRun)
        {
            // Draw
            alien_draw(generic_get_sprite_from_digit(digit_val), digit_location, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE * GLOBALS_SIZE_SCALAR, green);
        }

        score_copy /= DIGIT_OFFSET;
        digit_location -= GLOBALS_LETTER_WIDTH;
    }
}

// This function incs the lives
void ui_increment_lives()
 {
    if (lives + 1 <= MAX_LIVES) 
    {
        lives++;
        ui_update_lives(false);
    }
}

// This function decremnts the lives
void ui_decrement_lives()
 {
    lives--;
    ui_update_lives(false);
}

// This function updates the lives
// @param firstRun - if it's the first run or not
void ui_update_lives(bool firstRun) 
{
    uint32_t pos = GLOBALS_RIGHT_EDGE - GLOBALS_LETTER_WIDTH * WIDTH_SCALAR + GLOBALS_NEW_LINE * GLOBALS_SIZE_SCALAR;
    uint32_t initial_pos = pos;

    for (uint8_t l = 0; l < MAX_LIVES; l++)
     {
        alien_draw(tank_15x8, pos, GLOBALS_TANK_WIDTH, GLOBALS_TANK_HEIGHT, GLOBALS_PIXEL_SIZE, black);
        pos += GLOBALS_TANK_WIDTH * GLOBALS_PIXEL_SIZE + GLOBALS_TANK_WIDTH;
    }

    pos = initial_pos;
    for (uint8_t l = 0; l < lives; l++)
     {
        alien_draw(tank_15x8, pos, GLOBALS_TANK_WIDTH, GLOBALS_TANK_HEIGHT, GLOBALS_PIXEL_SIZE, cyan);
        pos += GLOBALS_TANK_WIDTH * GLOBALS_PIXEL_SIZE + GLOBALS_TANK_WIDTH;
    }
}