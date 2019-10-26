#include "init.h"
#include "../draw/bunker.h"
#include "../draw/generic.h"
#include "../draw/alien.h"
#include "../hdmi/hdmi.h"
#include "../draw/ui.h"
#include "../../Lab2/intc/intc.h"
#include "../../Lab2/buttons/buttons.h"
#include "../../Lab2/switches/switches.h"

/* Defines in this file */
#define ROWS_ON_SCREEN 80
#define DRAW_BOX_SCALAR 6
#define PIXEL_SIZE_SCALAR 8
#define BLACK_BOX_WIDTH 8
#define BLACK_BOX_HEIGHT 6
#define SCORE_S_SCALAR 0
#define SCORE_C_SCALAR 1
#define SCORE_O_SCALAR 2
#define SCORE_R_SCALAR 3
#define SCORE_E_SCALAR 4
#define LETTER_SCALAR 6
#define WIDTH_SCALAR 640*40
#define AAA_SCALAR 640*80
#define END_GAME_G 4
#define END_GAME_A 5
#define END_GAME_M 6
#define END_GAME_E 7
#define END_GAME_O 9
#define END_GAME_V 10
#define END_GAME_E2 11
#define END_GAME_R 12
#define ENTER_E 20
#define ENTER_N 21
#define ENTER_T 22
#define ENTER_E2 23
#define ENTER_R 24
#define NAME_N 26
#define NAME_A 27
#define NAME_M 28
#define NAME_E 29
#define AAA_1 24
#define AAA_2 25
#define AAA_3 26
#define INIT_L_SCALAR 15
#define INIT_I_SCALAR 14
#define INIT_V_SCALAR 13
#define INIT_E_SCALAR 12
#define INIT_S_SCALAR 11

/* Globals used in this file */
extern bool game_over;
extern char char_0;
extern char char_1;
extern char char_2;
extern char white[];
extern char cyan[];
extern char red[];
extern char magenta[];
extern char black[];
extern char white[];
extern char black_pixels[];

// This opens the hdmi fiel
void init_hdmi()
{
    hdmi_open(); // open hdmi file
}

// This function inits the screen
void init_screen()
{
    for (uint8_t row = 0; row < ROWS_ON_SCREEN; row++)
    {
        for (uint8_t col = 0; col < ROWS_ON_SCREEN; col++) {
            alien_draw(blackBox_8x6, 
            (row * GLOBALS_NEW_LINE * DRAW_BOX_SCALAR + col * GLOBALS_PIXEL_SIZE * PIXEL_SIZE_SCALAR), 
            BLACK_BOX_WIDTH, 
            BLACK_BOX_HEIGHT, 
            GLOBALS_SIZE_SCALAR * GLOBALS_PIXEL_SIZE, 
            black);
        }
    }
}

// This function inits the alien army on the screen, called again when all aliens die
void init_alien_army()
{
    uint32_t pos = GLOBALS_ALIEN_START_POS;
    for (uint16_t alien_y = 0; alien_y < GLOBALS_NO_ALIEN_Y; alien_y++)
    {
        for (uint32_t i = 0; i < GLOBALS_ALIEN_SPRITE_HEIGHT; i++)                                                                       
        {
            alien_draw_lots_o_aliens(pos, GLOBALS_ALIEN_SPRITE_WIDTH, i, alien_y, GLOBALS_PIXEL_SIZE * GLOBALS_SIZE_SCALAR, magenta, false);
            pos += GLOBALS_NEW_LINE * GLOBALS_SIZE_SCALAR;
            hdmi_seek(pos);
        }
        pos += GLOBALS_NEW_LINE * GLOBALS_MOVE_ROWS_DOWN_FOR_ALIENS;
        hdmi_seek(pos);
    }
}

// This function inits our black_pixels array
void init_black_pixels()
{
    // Inits black pixels array
    for (uint32_t i = 0; i < (GLOBALS_ALIEN_SPRITE_WIDTH + GLOBALS_SPACE_BW_ALIENS) * GLOBALS_SIZE_SCALAR * GLOBALS_PIXEL_SIZE * GLOBALS_NO_ALIEN_X; i++)
    {
        black_pixels[i] = GLOBALS_BLACK_PIXEL;
    }
}

// This draaws the zeros on the score
void init_score() 
{
    // Print SCORE
    alien_draw(letterS_5x5, GLOBALS_LETTER_WIDTH*SCORE_S_SCALAR, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    alien_draw(letterC_5x5, GLOBALS_LETTER_WIDTH*SCORE_C_SCALAR, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    alien_draw(letterO_5x5, GLOBALS_LETTER_WIDTH*SCORE_O_SCALAR, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    alien_draw(letterR_5x5, GLOBALS_LETTER_WIDTH*SCORE_R_SCALAR, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    alien_draw(letterE_5x5, GLOBALS_LETTER_WIDTH*SCORE_E_SCALAR, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);

    // Print Numbers
    ui_update_score(true);
}

// This draws and inits the lives to 3
void init_lives() 
{
    // Print LIVES
    alien_draw(letterL_5x5, GLOBALS_RIGHT_EDGE-GLOBALS_LETTER_WIDTH*INIT_L_SCALAR, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    alien_draw(letterI_5x5, GLOBALS_RIGHT_EDGE-GLOBALS_LETTER_WIDTH*INIT_I_SCALAR, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    alien_draw(letterV_5x5, GLOBALS_RIGHT_EDGE-GLOBALS_LETTER_WIDTH*INIT_V_SCALAR, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    alien_draw(letterE_5x5, GLOBALS_RIGHT_EDGE-GLOBALS_LETTER_WIDTH*INIT_E_SCALAR, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    alien_draw(letterS_5x5, GLOBALS_RIGHT_EDGE-GLOBALS_LETTER_WIDTH*INIT_S_SCALAR, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);

    ui_update_lives(false);
}

// This function inits the player and give a starting position
void init_player()
{
    alien_draw(tank_15x8, GLOBALS_PLAYER_START_POS, GLOBALS_TANK_WIDTH, GLOBALS_TANK_HEIGHT, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, cyan);
}

// This function ints the end game screen
void init_end_game() 
{
    init_screen();

    // GAME
    alien_draw(letterG_5x5, GLOBALS_PIXEL_SIZE*(GLOBALS_LETTER_WIDTH*END_GAME_G), GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*LETTER_SCALAR, white);
    alien_draw(letterA_5x5, GLOBALS_PIXEL_SIZE*(GLOBALS_LETTER_WIDTH*END_GAME_A), GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*LETTER_SCALAR, white);
    alien_draw(letterM_5x5, GLOBALS_PIXEL_SIZE*(GLOBALS_LETTER_WIDTH*END_GAME_M), GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*LETTER_SCALAR, white);
    alien_draw(letterE_5x5, GLOBALS_PIXEL_SIZE*(GLOBALS_LETTER_WIDTH*END_GAME_E), GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*LETTER_SCALAR, white);
    // OVER
    alien_draw(letterO_5x5, GLOBALS_PIXEL_SIZE*(GLOBALS_LETTER_WIDTH*END_GAME_O), GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*LETTER_SCALAR, white);
    alien_draw(letterV_5x5, GLOBALS_PIXEL_SIZE*(GLOBALS_LETTER_WIDTH*END_GAME_V), GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*LETTER_SCALAR, white);
    alien_draw(letterE_5x5, GLOBALS_PIXEL_SIZE*(GLOBALS_LETTER_WIDTH*END_GAME_E2), GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*LETTER_SCALAR, white);
    alien_draw(letterR_5x5, GLOBALS_PIXEL_SIZE*(GLOBALS_LETTER_WIDTH*END_GAME_R), GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*LETTER_SCALAR, white);

    // ENTER
    alien_draw(letterE_5x5, GLOBALS_LETTER_WIDTH*ENTER_E + WIDTH_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    alien_draw(letterN_5x5, GLOBALS_LETTER_WIDTH*ENTER_N + WIDTH_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    alien_draw(letterT_5x5, GLOBALS_LETTER_WIDTH*ENTER_T + WIDTH_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    alien_draw(letterE_5x5, GLOBALS_LETTER_WIDTH*ENTER_E2 + WIDTH_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    alien_draw(letterR_5x5, GLOBALS_LETTER_WIDTH*ENTER_R + WIDTH_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    // NAME
    alien_draw(letterN_5x5, GLOBALS_LETTER_WIDTH*NAME_N + WIDTH_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    alien_draw(letterA_5x5, GLOBALS_LETTER_WIDTH*NAME_A + WIDTH_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    alien_draw(letterM_5x5, GLOBALS_LETTER_WIDTH*NAME_M + WIDTH_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);
    alien_draw(letterE_5x5, GLOBALS_LETTER_WIDTH*NAME_E + WIDTH_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, white);

    // AAA
    alien_draw(generic_get_sprite_from_digit(char_0), GLOBALS_LETTER_WIDTH*AAA_1 + AAA_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, cyan);
    alien_draw(generic_get_sprite_from_digit(char_1), GLOBALS_LETTER_WIDTH*AAA_2 + AAA_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, cyan);
    alien_draw(generic_get_sprite_from_digit(char_2), GLOBALS_LETTER_WIDTH*AAA_3 + AAA_SCALAR*GLOBALS_PIXEL_SIZE, GLOBALS_LETTER_DIM, GLOBALS_LETTER_DIM, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, cyan);

    game_over = true;
}

// Main init function that inits everything
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
    alien_init();
    ui_init();
    init_player();
}