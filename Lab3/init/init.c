#include "init.h"
#include "../draw/bunker.h"

extern bool game_over;
extern char char_0;
extern char char_1;
extern char char_2;
extern char white[];
extern char cyan[];
extern char tan[];
extern char magenta[];
extern char black[];
extern char white[];
extern char black_pixels[];

/* Bunker attributes */

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
    for (uint16_t alien_y = 0; alien_y < NO_ALIEN_Y; alien_y++)
    {
        for (uint32_t i = 0; i < 8; i++)                                                                       
        {
            draw_lots_o_aliens(pos, ALIEN_SPRITE_WIDTH, i, alien_y, PIXEL_SIZE_GLOBAL * SIZE_SCALAR, magenta, false);
            pos += NEW_LINE * 2;
            seek_hdmi(pos);
        }
        pos += NEW_LINE * MOVE_ROWS_DOWN_FOR_ALIENS;
        seek_hdmi(pos);
    }
}

void init_black_pixels()
{
    // Inits black pixels
    for (uint32_t i = 0; i < (ALIEN_SPRITE_WIDTH + SPACE_BW_ALIENS) * SIZE_SCALAR * PIXEL_SIZE_GLOBAL * NO_ALIEN_X; i++)
    {
        black_pixels[i] = BLACK_PIXEL;
    }
}

void init_score() 
{
    // Print SCORE
    draw_alien(letterS_5x5, LETTER_WIDTH*0, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(letterC_5x5, LETTER_WIDTH*1, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(letterO_5x5, LETTER_WIDTH*2, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(letterR_5x5, LETTER_WIDTH*3, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(letterE_5x5, LETTER_WIDTH*4, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);

    // Print Numbers
    update_score(true);
}

void init_lives() 
{
    // Print LIVES
    draw_alien(letterL_5x5, RIGHT_EDGE-LETTER_WIDTH*15, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(letterI_5x5, RIGHT_EDGE-LETTER_WIDTH*14, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(letterV_5x5, RIGHT_EDGE-LETTER_WIDTH*13, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(letterE_5x5, RIGHT_EDGE-LETTER_WIDTH*12, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);
    draw_alien(letterS_5x5, RIGHT_EDGE-LETTER_WIDTH*11, LETTER_DIM, LETTER_DIM, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, white);

    draw_ui_update_lives(false);
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

// Main init function
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
    bunker_init();
    init_player();
}