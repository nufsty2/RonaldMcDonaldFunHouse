#include "draw_generic.h"
#include "../hdmi/hdmi.h"
#include "../sprites/sprites.c"

extern uint32_t debounce_ctr;
extern uint32_t increment_ctr;
extern char score_digit_0;
extern char score_digit_1;
extern char score_digit_2;
extern char score_digit_3;
extern char score_digit_4;
extern char char_0;
extern char char_1;
extern char char_2;
extern char selected_char;

void draw_sprite(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size, char color[]) 
{
    uint32_t init_point = pos; 
    uint32_t current_point = init_point;
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
                        current_point += (j*pixel_size + PIXEL_SIZE_GLOBAL*x + PIXEL_SIZE_GLOBAL*SCREEN_WIDTH*y);
                        seek_hdmi(current_point);
                        write_hdmi(color, PIXEL_SIZE_GLOBAL);
                        current_point = init_point;
                    }
                }
            }
        }
        current_point += (pixel_size*SCREEN_WIDTH); // new line
        init_point = current_point;
        seek_hdmi(current_point);
    }
}

const uint32_t* get_sprite_from_digit(char digit)
{
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
            printf("Hit default in get_sprite_from_digit()!\n\r");
            return letterA_5x5;
            break;
    }
}

// Reset all counters
void reset_counters() 
{
    debounce_ctr  = 0;
    increment_ctr = 0;
}