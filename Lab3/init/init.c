#include "../globals/globals.h"
#include "init.h"

void init_hdmi()
{
    open_hdmi(); // open hdmi file
}

void init_screen()
{
    for (uint32_t i = 0; i < WHOLE_SCREEN_IN_BYTES; i+=3)
    {
        seek_hdmi(i);
        write_hdmi(black, PIXEL_SIZE_GLOBAL);
    }
}

void init_alien_army()
{
    uint32_t pos = ALIEN_START_POS;
    for (uint16_t alien_y = 0; alien_y < 5; alien_y++)
    {
        for (uint32_t i = 0; i < 8; i++)                                                                       
        {
            draw_lots_o_aliens(pos, 13, i, alien_y, PIXEL_SIZE_GLOBAL * 2, magenta, false);
            pos += NEW_LINE * 2;
            seek_hdmi(pos);
        }
        pos += NEW_LINE * 15;
        seek_hdmi(pos);
    }
}

void init_black_pixels()
{
    // Inits black pixels
    for (uint32_t i = 0; i < (13 + SPACE_BW_ALIENS) * 2 * PIXEL_SIZE_GLOBAL * 11; i++)
    {
        black_pixels[i] = 0x00;
    }
}

void init_score() 
{
    // Print SCORE
    draw_alien(letterS_5x5, LETTER_WIDTH*0, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(letterC_5x5, LETTER_WIDTH*1, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(letterO_5x5, LETTER_WIDTH*2, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(letterR_5x5, LETTER_WIDTH*3, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(letterE_5x5, LETTER_WIDTH*4, 5, 5, PIXEL_SIZE_GLOBAL*2, white);

    // Print Numbers
    update_score(true);
}

void init_lives() 
{
    // Print LIVES
    draw_alien(letterL_5x5, RIGHT_EDGE-LETTER_WIDTH*15, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(letterI_5x5, RIGHT_EDGE-LETTER_WIDTH*14, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(letterV_5x5, RIGHT_EDGE-LETTER_WIDTH*13, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(letterE_5x5, RIGHT_EDGE-LETTER_WIDTH*12, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(letterS_5x5, RIGHT_EDGE-LETTER_WIDTH*11, 5, 5, PIXEL_SIZE_GLOBAL*2, white);

    update_lives(false);
}

void init_bunkers()
{
    uint32_t pos = PIXEL_SIZE_GLOBAL*(640*400 + 75);
    // Print bunkers
    for (uint8_t i = 0; i < 4; i++) {
        draw_alien(bunker_24x18, pos, 24, 18, PIXEL_SIZE_GLOBAL*2, tan);
        pos += (PIXEL_SIZE_GLOBAL * 24) * 6;
    }
}

void init_player()
{
    draw_alien(tank_15x8, PLAYER_START_POS, 15, 8, PIXEL_SIZE_GLOBAL*2, cyan);
}

void init_end_game() 
{
    init_screen();

    // GAME
    draw_alien(letterG_5x5, PIXEL_SIZE_GLOBAL*(LETTER_WIDTH*4), 5, 5, PIXEL_SIZE_GLOBAL*6, white);
    draw_alien(letterA_5x5, PIXEL_SIZE_GLOBAL*(LETTER_WIDTH*5), 5, 5, PIXEL_SIZE_GLOBAL*6, white);
    draw_alien(letterM_5x5, PIXEL_SIZE_GLOBAL*(LETTER_WIDTH*6), 5, 5, PIXEL_SIZE_GLOBAL*6, white);
    draw_alien(letterE_5x5, PIXEL_SIZE_GLOBAL*(LETTER_WIDTH*7), 5, 5, PIXEL_SIZE_GLOBAL*6, white);
    // OVER
    draw_alien(letterO_5x5, PIXEL_SIZE_GLOBAL*(LETTER_WIDTH*9), 5, 5, PIXEL_SIZE_GLOBAL*6, white);
    draw_alien(letterV_5x5, PIXEL_SIZE_GLOBAL*(LETTER_WIDTH*10), 5, 5, PIXEL_SIZE_GLOBAL*6, white);
    draw_alien(letterE_5x5, PIXEL_SIZE_GLOBAL*(LETTER_WIDTH*11), 5, 5, PIXEL_SIZE_GLOBAL*6, white);
    draw_alien(letterR_5x5, PIXEL_SIZE_GLOBAL*(LETTER_WIDTH*12), 5, 5, PIXEL_SIZE_GLOBAL*6, white);

    // ENTER
    draw_alien(letterE_5x5, LETTER_WIDTH*20 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(letterN_5x5, LETTER_WIDTH*21 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(letterT_5x5, LETTER_WIDTH*22 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(letterE_5x5, LETTER_WIDTH*23 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(letterR_5x5, LETTER_WIDTH*24 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    // NAME
    draw_alien(letterN_5x5, LETTER_WIDTH*26 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(letterA_5x5, LETTER_WIDTH*27 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(letterM_5x5, LETTER_WIDTH*28 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(letterE_5x5, LETTER_WIDTH*29 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, white);

    // AAA
    draw_alien(get_sprite_from_digit(char_0), LETTER_WIDTH*24 + 640*80*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, cyan);
    draw_alien(get_sprite_from_digit(char_1), LETTER_WIDTH*25 + 640*80*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, cyan);
    draw_alien(get_sprite_from_digit(char_2), LETTER_WIDTH*26 + 640*80*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, cyan);

    game_over = true;
}

void init()
{
        // Init INTC and GPIO Devices
    intc_init("/dev/uio4"); // init the interrupt controller driver
    init_buttons("/dev/uio1"); // Initialize buttons 
    init_switches("/dev/uio2"); // inits the switches

    // Init black pixels
    init_black_pixels();

    // init screen stuff
    init_hdmi();
    init_screen();
    init_alien_army();
    init_score();
    init_lives();
    init_bunkers();
    init_player();
}