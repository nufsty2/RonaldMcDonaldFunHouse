#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../hdmi/hdmi.h"
#include "../scores/scores.h"
#include "../sprites/sprites.c"
#include "../../Lab2/intc/intc.h"
#include "../../Lab2/buttons/buttons.h"
#include "../../Lab2/switches/switches.h"
 
#define ALIEN_HEIGHT 16

#define WORD_WIDTH 32
#define PIXEL_SIZE_GLOBAL 3
#define LETTER_WIDTH 39

#define SCORE_DIGITS 5

/* Which interrupts are enabled masks */
#define FIT_MASK 0x1 // 001
#define BTN_MASK 0x2 // 010
#define SW_MASK  0x4 // 100

/* Useful bitmasks for the switches */
#define BTN_0 0x1
#define BTN_1 0x2
#define BTN_2 0x4
#define BTN_3 0x8
#define SW_0  0x1

/* values */
#define DEBOUNCE_MAX_VAL   5
#define HALF_SECOND        50
#define INCREMENT_MAX_VAL  10
#define FIT_1_SEC          100 // 10ms * 100  1000ms = 1s
#define ALIEN_MOVE_MAX_VAL 10


/* defines I made */
#define WHOLE_SCREEN_IN_BYTES 921600
#define RIGHT_EDGE (NEW_LINE - 3)
#define NEW_LINE (640 * PIXEL_SIZE_GLOBAL)
#define ALIEN_START_POS 105 + NEW_LINE*105

static char   white[3]  = {0xFF, 0xFF, 0xFF};
static char   green[3]  = {0x00, 0xFF, 0x77};
static char   black[3]  = {0x00, 0x00, 0x00};
static char    cyan[3]  = {0x11, 0xFF, 0xFF};
static char     tan[3]  = {0xFF, 0x55, 0x55};
static char magenta[3]  = {0xFF, 0x66, 0xFF};

static uint32_t debounce_ctr     = 0; // debounce counter
static uint32_t buttons_val      = 0; // buttons value
static uint32_t new_buttons_val  = 0; // new buttons value
static uint32_t switches_val     = 0; // switches value
static uint32_t new_switches_val = 0; // new switches value
static uint32_t fit_ctr       =  0;
static uint32_t half_sec_ctr  =  0;
static uint32_t increment_ctr =  0;
static uint32_t alien_move_ctr = 0;

static bool debounced = false; // Flag used to determine if debounce timer is done
static bool blink = true;
static bool game_over = false;
static bool name_entered = false;

const uint32_t* alien_army_sprites[5][11] = {
    {alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8,    alien_top_in_12x8   },
    {alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8},
    {alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8, alien_middle_in_12x8},
    {alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8},
    {alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8, alien_bottom_in_12x8}
};

bool alien_army_is_alive[5][11] = {
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true}
};

bool alien_army_is_in[5][11] = {
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true}
};

uint32_t score = 0;
char score_digit_0 = 0;
char score_digit_1 = 0;
char score_digit_2 = 0;
char score_digit_3 = 0;
char score_digit_4 = 0;

char char_0 = 'A';
char char_1 = 'B';
char char_2 = 'C';
uint8_t selected_char = 0;

uint8_t lives = 3;
uint32_t current_pos = ALIEN_START_POS;
bool moving_right = true;

void init_hdmi()
{
    open_hdmi();
}

void black_whole_screen()
{
    for (uint32_t i = 0; i < WHOLE_SCREEN_IN_BYTES; i+=3)
    {
        seek_hdmi(i);
        write_hdmi(black, PIXEL_SIZE_GLOBAL);
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
                        write_hdmi(color, PIXEL_SIZE_GLOBAL);
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

void draw_alien(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size, char color[]) {
    uint32_t initPoint = pos; 
    uint32_t currentPoint = initPoint;
    uint32_t grid_dimension = pixel_size / PIXEL_SIZE_GLOBAL;

    for (uint32_t i = 0; i < height; i++)
    {
        char line_of_pixels[width * grid_dimension * PIXEL_SIZE_GLOBAL];
        for (uint32_t j = 0; j < width; j++)
        {
            for (uint32_t x = j * PIXEL_SIZE_GLOBAL * grid_dimension; x < (j + 1) * grid_dimension * PIXEL_SIZE_GLOBAL; x+=PIXEL_SIZE_GLOBAL) 
            {
                line_of_pixels[x + 0] = (sprite[i] & (1<<(width-1-j))) ? color[0] : 0x00;
                line_of_pixels[x + 1] = (sprite[i] & (1<<(width-1-j))) ? color[1] : 0x00;
                line_of_pixels[x + 2] = (sprite[i] & (1<<(width-1-j))) ? color[2] : 0x00;
            }
        }
        for (uint32_t y = 0; y < grid_dimension; y++) 
        {
            seek_hdmi(currentPoint+(NEW_LINE)*y);
            write_hdmi(line_of_pixels, width * grid_dimension * PIXEL_SIZE_GLOBAL);
        }
        currentPoint += NEW_LINE * grid_dimension;
        seek_hdmi(currentPoint);
    }
}

void toggle_alien_sprite(const uint32_t* sprite_val, int16_t x, int16_t y) {
    if (sprite_val == alien_top_in_12x8)
    {
        alien_army_sprites[y][x] = alien_top_out_12x8;
    }
        
    else if (sprite_val == alien_top_out_12x8)
    {
        alien_army_sprites[y][x] = alien_top_in_12x8;
    }
        
    else if (sprite_val == alien_middle_in_12x8)
    {
        alien_army_sprites[y][x] = alien_middle_out_12x8;
    }
        
    else if (sprite_val == alien_middle_out_12x8)
    {
        alien_army_sprites[y][x] = alien_middle_in_12x8;
    }
        
    else if (sprite_val == alien_bottom_in_12x8)
    {
        alien_army_sprites[y][x] = alien_bottom_out_12x8;
    }
        
    else if (sprite_val == alien_bottom_out_12x8)
    {
        alien_army_sprites[y][x] = alien_bottom_in_12x8;
    }          
}

void move_alien_army_horizontal(bool right) {
    for (uint16_t y = 0; y < 5; y++) 
    {
        for (uint16_t x = 0; x < 11; x++) 
        {
            // Erase old
            draw_alien(alien_army_sprites[y][x], 
                        current_pos + 40*PIXEL_SIZE_GLOBAL*x + 25*640*PIXEL_SIZE_GLOBAL*y,
                        12,
                        8,
                        PIXEL_SIZE_GLOBAL * 2,
                        black
            );

            // Change sprite
            toggle_alien_sprite(alien_army_sprites[y][x], x, y);

            // Redraw
            draw_alien(alien_army_sprites[y][x], 
                        current_pos + PIXEL_SIZE_GLOBAL * (2 + 40*x + 25*640*y),
                        12,
                        8,
                        PIXEL_SIZE_GLOBAL * 2,
                        magenta
            );
        }
    }
    current_pos += PIXEL_SIZE_GLOBAL * 2;
}

void init_alien_army()
{
    for (uint16_t y = 0; y < 1; y++) 
    {
        for (uint16_t x = 0; x < 1; x++) 
        {
            draw_alien(alien_army_sprites[y][x], 
                        ALIEN_START_POS + 40*PIXEL_SIZE_GLOBAL*x + 25*640*PIXEL_SIZE_GLOBAL*y,
                        12,
                        8,
                        PIXEL_SIZE_GLOBAL * 2,
                        magenta
            );
        }
    }
}

void init_alien_army_true()
{
    for (uint16_t y = 0; y < 5; y++) 
    {
        for (uint16_t x = 0; x < 11; x++) 
        {
            draw_alien(alien_army_sprites[y][x], 
                        ALIEN_START_POS + 40*PIXEL_SIZE_GLOBAL*x + 25*640*PIXEL_SIZE_GLOBAL*y,
                        12,
                        8,
                        PIXEL_SIZE_GLOBAL * 2,
                        green
            );
        }
    }
}

const uint32_t* get_sprite_from_digit(char digit) {
    switch (digit) {
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
            return NULL;
    }
}

char* get_score_digit(uint8_t digit) {
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

char* get_selected_char() {
    switch(selected_char) {
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

void update_score(bool firstRun) {
    uint32_t score_copy = score;
    uint32_t digit_location = LETTER_WIDTH * 10;

    // Print numbers
    for (uint16_t i = 0; i < SCORE_DIGITS; i++)
    {
        uint8_t digit_val = score_copy % 10;

        if ((*get_score_digit(i) != digit_val) && !firstRun)
        {
            // Erase
            draw_alien(get_sprite_from_digit(*get_score_digit(i)), digit_location, 5, 5, PIXEL_SIZE_GLOBAL * 2, black);
            // Draw
            draw_alien(get_sprite_from_digit(digit_val), digit_location, 5, 5, PIXEL_SIZE_GLOBAL * 2, green);
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

void update_lives(bool firstRun) {
    uint32_t pos = RIGHT_EDGE - LETTER_WIDTH * 9 + 640 * 3 * 2;
    for (uint8_t l = 0; l < lives; l++) {
        draw_alien(tank_15x8, pos, 15, 8, PIXEL_SIZE_GLOBAL, cyan);
        pos += 15 * PIXEL_SIZE_GLOBAL + 15;
    }
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
    draw_alien(tank_15x8, PIXEL_SIZE_GLOBAL*(640*460 + 10), 15, 8, PIXEL_SIZE_GLOBAL*2, cyan);
}

void init_end_game() 
{
    black_whole_screen();

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

void blink_cursor(bool force) 
{
    draw_alien(letterBLOCK_6x6, 
                LETTER_WIDTH*24 - 3 + (selected_char * LETTER_WIDTH) + 640*79*PIXEL_SIZE_GLOBAL, 
                6, 
                6, 
                PIXEL_SIZE_GLOBAL*2, 
                force ? black : 
                    blink ? cyan : black
    );
    draw_alien(get_sprite_from_digit(*get_selected_char()),     
                LETTER_WIDTH*24 +     (selected_char * LETTER_WIDTH) + 640*80*PIXEL_SIZE_GLOBAL, 
                5, 
                5, 
                PIXEL_SIZE_GLOBAL*2, 
                force ? cyan : 
                    blink ? black : cyan
    );  
    blink = !blink;
}

// Reset all counters
void reset_counters() 
{
    debounce_ctr  = 0;
    increment_ctr = 0;
}

void main_print_score(score_t score, uint32_t pos) {
    draw_alien(get_sprite_from_digit(score.name[0]),       pos + LETTER_WIDTH*0, 5, 5, PIXEL_SIZE_GLOBAL*2, cyan);
    draw_alien(get_sprite_from_digit(score.name[1]),       pos + LETTER_WIDTH*1, 5, 5, PIXEL_SIZE_GLOBAL*2, cyan);
    draw_alien(get_sprite_from_digit(score.name[2]),       pos + LETTER_WIDTH*2, 5, 5, PIXEL_SIZE_GLOBAL*2, cyan);
    draw_alien(get_sprite_from_digit(score.value_char[0]), pos + LETTER_WIDTH*4, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(get_sprite_from_digit(score.value_char[1]), pos + LETTER_WIDTH*5, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(get_sprite_from_digit(score.value_char[2]), pos + LETTER_WIDTH*6, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(get_sprite_from_digit(score.value_char[3]), pos + LETTER_WIDTH*7, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
    draw_alien(get_sprite_from_digit(score.value_char[4]), pos + LETTER_WIDTH*8, 5, 5, PIXEL_SIZE_GLOBAL*2, white);
}

void main_print_scores()
{
    // ENTER
    draw_alien(letterE_5x5, LETTER_WIDTH*20 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(letterN_5x5, LETTER_WIDTH*21 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(letterT_5x5, LETTER_WIDTH*22 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(letterE_5x5, LETTER_WIDTH*23 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(letterR_5x5, LETTER_WIDTH*24 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    // NAME
    draw_alien(letterN_5x5, LETTER_WIDTH*26 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(letterA_5x5, LETTER_WIDTH*27 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(letterM_5x5, LETTER_WIDTH*28 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(letterE_5x5, LETTER_WIDTH*29 + 640*40*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    // AAA
    draw_alien(get_sprite_from_digit(char_0), LETTER_WIDTH*24 + 640*80*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(get_sprite_from_digit(char_1), LETTER_WIDTH*25 + 640*80*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);
    draw_alien(get_sprite_from_digit(char_2), LETTER_WIDTH*26 + 640*80*PIXEL_SIZE_GLOBAL, 5, 5, PIXEL_SIZE_GLOBAL*2, black);

    score_t top_ten[10];
    scores_get_top_10_scores(top_ten);
    for (uint8_t i = 0; i < 10; i++) 
    {
        main_print_score(top_ten[i], PIXEL_SIZE_GLOBAL*((80+20*i)*640) + LETTER_WIDTH*20);
    }
}

// BTN0: Increase letter
// BTN1: Decrease letter
// BTN3: Submit letter
void respond_to_press() {
    if (buttons_val == BTN_0) 
    {
        char val = *get_selected_char(selected_char);

        if (++val > 'Z')
            *get_selected_char(selected_char) = 'A';
        else
            *get_selected_char(selected_char) += 1;
    }

    else if (buttons_val == BTN_1)
    {
        char val = *get_selected_char(selected_char);

        if (--val < 'A')
            *get_selected_char(selected_char) = 'Z';
        else
            *get_selected_char(selected_char) -= 1;
    }

    else if (buttons_val == BTN_3)
    {
        if (++selected_char > 2) {
           name_entered = true;
           score_t new_score = {.name = {char_0, char_1, char_2}, .value = score};
           scores_write_new_score(new_score);
           main_print_scores();
        }
    }

    if (selected_char <= 2) 
    {
        blink_cursor(true);
        half_sec_ctr = 0;
        blink = true;
    }   
}

// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce switches, and 2) advances the time.
void isr_fit()
{
    if (++debounce_ctr >= DEBOUNCE_MAX_VAL)
    {
        debounce_ctr = 0; // reset debounce counter when max value hit
        buttons_val = new_buttons_val; // assign the buttons val
        switches_val = new_switches_val; // assign the switches vals
        debounced = true;
    }

    // The time will auto-increment if pressed for 1/2 second
    if ((++half_sec_ctr >= HALF_SECOND) && game_over && !name_entered) {
        blink_cursor(false);
        half_sec_ctr = 0;
    }

    if ((++alien_move_ctr >= ALIEN_MOVE_MAX_VAL) && !game_over) {
        move_alien_army_horizontal(moving_right);
        alien_move_ctr = 0;
    }
}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void isr_buttons()
{
    reset_counters();
    new_buttons_val = read_buttons(); // get the button value
    clear_button_interrupts(); // clear button interrupts
    
    if (debounced) 
    {
        if (game_over && !name_entered) 
            respond_to_press();

        else
            init_end_game();

        debounced = false;
    }
}

// This is invoked each time there is a change in switch state
void isr_switches()
{
    reset_counters();
    new_switches_val = read_switches(); // read the switches
    clear_switches_interrupts(); // clear the interrupts
}

// Init function to init all different files
void init()
{
    intc_init("/dev/uio4"); // init the interrupt controller driver
    init_buttons("/dev/uio1"); // Initialize buttons 
    init_switches("/dev/uio2"); // inits the switches
}
     
int main() 
{
    init_hdmi();
    black_whole_screen();
    init_alien_army();
    // init_alien_army_true();
    init_score();
    init_lives();
    init_bunkers();
    init_player();

    init(); // INITS EVERYTHING
        
    // Enable all interrupts
    intc_enable_uio_interrupts();
    enable_button_interrupts();
    enable_switches_interrupts();

    fit_ctr = 0; // reset fit counter

    while(!name_entered) 
    {
        // Call interrupt controller function to wait for interrupt
        uint32_t num_interrupts = intc_wait_for_interrupt(); // wait
        uint32_t interrupt_value = intc_get_interrupt_val(); // get the ISR
            
        if (interrupt_value & FIT_MASK) // if fit is enabled go into isr_fit
        {
            isr_fit();
        }

        if (interrupt_value & BTN_MASK) // if button mask detected an interrupt, go into usr_buttons()
        {
            isr_buttons();
        }

        if (interrupt_value & SW_MASK) // if the switch detects an interrupt, go into isr_switches
        {
            isr_switches();
        }

        intc_ack_interrupt(interrupt_value); // acknowledge what was pressed
        intc_enable_uio_interrupts(); // re enables interrupts
    }

    // EXIT
    buttons_exit(); // exit the buttons
    switches_exit(); // exit the switches
    intc_exit(); // exits the int controller
}