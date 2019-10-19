#include "generic.h"
#include "../hdmi/hdmi.h"
#include "../sprites/sprites.c"

/* Counters */
extern uint32_t debounce_ctr;
extern uint32_t increment_ctr;

/* Score digits */
extern char score_digit_0;
extern char score_digit_1;
extern char score_digit_2;
extern char score_digit_3;
extern char score_digit_4;

/* Chars on the final score */
extern char char_0;
extern char char_1;
extern char char_2;

/* Currently selected score */
extern char selected_char;

// This is our draw sprite function we use everywhere except for drawing the alien
// @param sprite - the sprite we want to draw
// @param pos - the position we want to draw on the screen
// @param width - width of the sprite
// @param height - height of the sprite
// @param pixel_size - the scalar we want on the sprite
// @param color - color of the sprite
void generic_draw_sprite(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size, char color[]) 
{
    // Set the points to seek
    uint32_t init_point = pos; 
    uint32_t current_point = init_point;

    // Get the scalar size
    uint32_t grid_dimension = pixel_size / PIXEL_SIZE_GLOBAL;

    for (uint32_t i = 0; i < height; i++) // loop through the height of sprite
    {
        for (uint32_t j = 0; j < width; j++) // loop through width of sprite
        {
            if ((sprite[i] & (1<<(width-1-j)))) // if there is a 1 at that sprite
            {
                for (uint32_t y = 0; y < grid_dimension; y++)  // loop through the grid dimesnion
                {
                    for (uint32_t x = 0; x < grid_dimension; x++) // again
                    {
                        current_point += (j*pixel_size + PIXEL_SIZE_GLOBAL*x + PIXEL_SIZE_GLOBAL*SCREEN_WIDTH*y); // set current point
                        hdmi_seek(current_point); // seek to that point
                        hdmi_write(color, PIXEL_SIZE_GLOBAL); // write to that point
                        current_point = init_point; // init point
                    }
                }
            }
        }

        // New line
        current_point += (pixel_size*SCREEN_WIDTH);
        init_point = current_point;
        hdmi_seek(current_point);
    }
}

// This function gets a digit sprite from the digit we pass in
// @param digit - the digit we want the sprite for
// return - returns the corresponding sprite for that digit
const uint32_t* generic_get_sprite_from_digit(char digit)
{
    // Determine the digit
    switch (digit) 
    {
        case 0:
            return number0_5x5;
        case 1:
            return number1_5x5;
        case 2:
            return number2_5x5;
        case 3:
            return number3_5x5;
        case 4:
            return number4_5x5;
        case 5:
            return number5_5x5;
        case 6:
            return number6_5x5;
        case 7:
            return number7_5x5;
        case 8:
            return number8_5x5;
        case 9:
            return number9_5x5;
        case '0':
            return number0_5x5;
        case '1':
            return number1_5x5;
        case '2':
            return number2_5x5;
        case '3':
            return number3_5x5;
        case '4':
            return number4_5x5;
        case '5':
            return number5_5x5;
        case '6':
            return number6_5x5;
        case '7':
            return number7_5x5;
        case '8':
            return number8_5x5;
        case '9':
            return number9_5x5;
        case 'A':
            return letterA_5x5;
        case 'B':
            return letterB_5x5;
        case 'C':
            return letterC_5x5;
        case 'D':
            return letterD_5x5;
        case 'E':
            return letterE_5x5;
        case 'F':
            return letterF_5x5;
        case 'G':
            return letterG_5x5;
        case 'H':
            return letterH_5x5;
        case 'I':
            return letterI_5x5;
        case 'J':
            return letterJ_5x5;
        case 'K':
            return letterK_5x5;
        case 'L':
            return letterL_5x5;
        case 'M':
            return letterM_5x5;
        case 'N':
            return letterN_5x5;
        case 'O':
            return letterO_5x5;
        case 'P':
            return letterP_5x5;
        case 'Q':
            return letterQ_5x5;
        case 'R':
            return letterR_5x5;
        case 'S':
            return letterS_5x5;
        case 'T':
            return letterT_5x5;
        case 'U':
            return letterU_5x5;
        case 'V':
            return letterV_5x5;
        case 'W':
            return letterW_5x5;
        case 'X':
            return letterX_5x5;
        case 'Y':
            return letterY_5x5;
        case 'Z':
            return letterZ_5x5;
        default:
            printf("Hit default in generic_get_sprite_from_digit()!\n\r"); // debug
            return letterA_5x5; // return a letter a
    }
}

// Reset all counters
void generic_reset_counters() 
{
    debounce_ctr  = 0;
    increment_ctr = 0;
}