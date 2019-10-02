#include <stdio.h>
#include <stdint.h>
#include "../hdmi/hdmi.h"
#include "../sprites/sprites.c"
#include <stdbool.h>
 
#define ALIEN_HEIGHT 16

static char white[3] = {0xFF, 0xFF, 0xFF};
static char green[3] = {0x00, 0xFF, 0x77};
static char black[3] = {0x00, 0x00, 0x00};

 
#define WORD_WIDTH 32
#define PIXEL_SIZE_GLOBAL 3
#define LETTER_WIDTH 39

#define SCORE_DIGITS 5

/* defines I made */
#define WHOLE_SCREEN_IN_BYTES 921600

const uint32_t* alien_army[5][11] = {
    {alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8   },
    {alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8},
    {alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8},
    {alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8},
    {alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8}
};

uint32_t score = 0;
char score_digit_0 = 0;
char score_digit_1 = 0;
char score_digit_2 = 0;
char score_digit_3 = 0;
char score_digit_4 = 0;

void init_hdmi()
{
    open_hdmi();
}

void black_whole_screen()
{
    for (uint32_t i = 0; i < WHOLE_SCREEN_IN_BYTES; i+=3)
    {
        seek_hdmi(i);
        write_hdmi(black);
    }
}

void draw_sprite(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size, char color[]) {
    uint32_t initPoint = pos; 
    uint32_t currentPoint = initPoint;
    uint32_t grid_dimension = pixel_size / PIXEL_SIZE_GLOBAL;

    for (uint32_t i = 0; i < height; i++)
    {
        for (uint32_t j = 0; j < width; j++)
        {
            if ((sprite[i] & (1<<(width-1-j))))
            {
                for (uint32_t y = 0; y < grid_dimension; y++) 
                {
                    for (uint32_t x = 0; x < grid_dimension; x++) 
                    {
                    currentPoint += (j*pixel_size + PIXEL_SIZE_GLOBAL*x + PIXEL_SIZE_GLOBAL*640*y);
                    seek_hdmi(currentPoint);
                    write_hdmi(color);
                    currentPoint = initPoint;
                    }
                }
            }
        }
        currentPoint += (pixel_size*640); // new line
        initPoint = currentPoint;
        seek_hdmi(currentPoint);
    }
}

void init_alien_army()
{
    uint32_t starting_pos = 105 + 640*PIXEL_SIZE_GLOBAL*105;

    for (uint16_t y = 0; y < 5; y++) 
    {
        for (uint16_t x = 0; x < 11; x++) {
            draw_sprite(alien_army[y][x], 
                        starting_pos + 40*PIXEL_SIZE_GLOBAL*x + 25*640*PIXEL_SIZE_GLOBAL*y,
                        12,
                        8,
                        PIXEL_SIZE_GLOBAL * 2,
                        white
            );
        }
    }
}

uint32_t* get_sprite_from_digit(uint8_t digit) {
    switch (digit) {
        case 0:
            return number0_5x5;
        case 1:
            return number0_5x5;
        case 2:
            return number0_5x5;
        case 3:
            return number0_5x5;
        case 4:
            return number0_5x5;
        case 5:
            return number0_5x5;
        case 6:
            return number0_5x5;
        case 7:
            return number0_5x5;
        case 8:
            return number0_5x5;
        case 9:
            return number0_5x5;
    }
}

char* get_score_digit(uint16_t digit) {
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

void update_score(bool firstRun) {
    uint32_t score_copy = score;
    uint32_t digit_location = LETTER_WIDTH * 9;

    // Print numbers
    for (uint16_t i = 0; i < SCORE_DIGITS; i++)
    {
        uint8_t digit_val = score_copy % 10;

        if (*get_score_digit(i) != digit_val && !firstRun)
        {
            // Erase
            draw_sprite(get_sprite_from_digit(*get_score_digit(i)), digit_location, 5, 5, PIXEL_SIZE_GLOBAL * 2, black);
            // Draw
            draw_sprite(get_sprite_from_digit(digit_val), digit_location, 5, 5, PIXEL_SIZE_GLOBAL * 2, green);
        }
        else 
        {
            // Draw
            draw_sprite(get_sprite_from_digit(digit_val), digit_location, 5, 5, PIXEL_SIZE_GLOBAL * 2, green);
        }

        *get_score_digit(i) = digit_val;
        score_copy /= 10;
        digit_location -= LETTER_WIDTH;
    }
}

void init_score() 
{
    // Print SCORE
    draw_sprite(letterS_5x5, LETTER_WIDTH*0, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_sprite(letterC_5x5, LETTER_WIDTH*1, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_sprite(letterO_5x5, LETTER_WIDTH*2, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_sprite(letterR_5x5, LETTER_WIDTH*3, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_sprite(letterE_5x5, LETTER_WIDTH*4, 5, 5, PIXEL_SIZE_GLOBAL*2, white);

    // Print Numbers
    update_score(true);
}
 
int main() 
{
    init_hdmi();
    black_whole_screen();

    init_alien_army();
    init_score();
}