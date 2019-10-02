#include <stdio.h>
#include <stdint.h>
#include "../hdmi/hdmi.h"
#include "../sprites/sprites.c"
 
#define ALIEN_HEIGHT 16

static char white[3] = {0xFF, 0xFF, 0xFF};                                                                                                                                                                      
 
#define WORD_WIDTH 32
#define PIXEL_SIZE_GLOBAL 3
#define LETTER_WIDTH 39

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

void init_hdmi()
{
    open_hdmi();
}

void black_whole_screen()
{
    char black[3] = {0x00, 0x00, 0x00};
    for (uint32_t i = 0; i < WHOLE_SCREEN_IN_BYTES; i+=3)
    {
        seek_hdmi(i);
        write_hdmi(black);
    }
}

void draw_sprite(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size) {
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
                    write_hdmi(white);
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
                        PIXEL_SIZE_GLOBAL * 2);
        }
    }
}

void draw_score() 
{
    draw_sprite(letterS_5x5, LETTER_WIDTH*0, 5, 5, PIXEL_SIZE_GLOBAL*2);
    draw_sprite(letterC_5x5, LETTER_WIDTH*1, 5, 5, PIXEL_SIZE_GLOBAL*2);
    draw_sprite(letterO_5x5, LETTER_WIDTH*2, 5, 5, PIXEL_SIZE_GLOBAL*2);
    draw_sprite(letterR_5x5, LETTER_WIDTH*3, 5, 5, PIXEL_SIZE_GLOBAL*2);
    draw_sprite(letterE_5x5, LETTER_WIDTH*4, 5, 5, PIXEL_SIZE_GLOBAL*2);
    
    
}
 
int main() 
{
    init_hdmi();
    black_whole_screen();

    init_alien_army();
    draw_score();
}