#include "draw.h"

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

void draw_lots_o_aliens(uint32_t pos, uint32_t width, uint32_t sprite_row, uint32_t alien_y, uint16_t pixel_size, char color[], bool erase_aliens)
{
    uint32_t currentPoint = pos;
    uint32_t grid_dimension = pixel_size / PIXEL_SIZE_GLOBAL;

    char pixels_for_entire_line[(width + SPACE_BW_ALIENS) * grid_dimension * PIXEL_SIZE_GLOBAL * 11]; 
    if (!erase_aliens)
    {
        for (uint16_t alien_x = 0; alien_x < 11; alien_x++)                                                        
        {
            const uint32_t* sprite = alien_army_sprites[alien_y][alien_x];                                          
            for (uint32_t j = (alien_x*(width+SPACE_BW_ALIENS)); j < (alien_x+1) * (width + SPACE_BW_ALIENS); j++)          
            {
                // if (alien_x >= 0) { printf("J: %d\n\r Alien_x: %d\n\r", j, alien_x); }
                for (uint32_t x = j * PIXEL_SIZE_GLOBAL * grid_dimension; x < (j + 1) * grid_dimension * PIXEL_SIZE_GLOBAL; x+=PIXEL_SIZE_GLOBAL)
                {
                    pixels_for_entire_line[x + 0] = (((j >= alien_x*(width+SPACE_BW_ALIENS)) && (j < width*(alien_x+1)+(SPACE_BW_ALIENS*alien_x))) && (sprite[sprite_row] & (1<<(width-1-(j-(width+SPACE_BW_ALIENS)*alien_x))))) ? color[0] : 0x00;
                    pixels_for_entire_line[x + 1] = (((j >= alien_x*(width+SPACE_BW_ALIENS)) && (j < width*(alien_x+1)+(SPACE_BW_ALIENS*alien_x))) && (sprite[sprite_row] & (1<<(width-1-(j-(width+SPACE_BW_ALIENS)*alien_x))))) ? color[1] : 0x00;
                    pixels_for_entire_line[x + 2] = (((j >= alien_x*(width+SPACE_BW_ALIENS)) && (j < width*(alien_x+1)+(SPACE_BW_ALIENS*alien_x))) && (sprite[sprite_row] & (1<<(width-1-(j-(width+SPACE_BW_ALIENS)*alien_x))))) ? color[2] : 0x00;
                }
            }
        }
    }

    for (uint32_t y = 0; y < grid_dimension; y++) 
    {        
        seek_hdmi(currentPoint+(NEW_LINE)*y);
        write_hdmi((erase_aliens) ? black_pixels : pixels_for_entire_line, (width + SPACE_BW_ALIENS) * grid_dimension * PIXEL_SIZE_GLOBAL * 11);
    }
}

void draw_alien(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size, char color[])
{
    uint32_t initPoint = pos; 
    uint32_t currentPoint = initPoint;
    uint32_t grid_dimension = pixel_size / PIXEL_SIZE_GLOBAL;

    for (uint32_t i = 0; i < height; i++)
    {
        char pixels_for_sprite_line[width * grid_dimension * PIXEL_SIZE_GLOBAL];
        for (uint32_t j = 0; j < width; j++)
        {
            for (uint32_t x = j * PIXEL_SIZE_GLOBAL * grid_dimension; x < (j + 1) * grid_dimension * PIXEL_SIZE_GLOBAL; x+=PIXEL_SIZE_GLOBAL) 
            {
                pixels_for_sprite_line[x + 0] = (sprite[i] & (1<<(width-1-j))) ? color[0] : 0x00;
                pixels_for_sprite_line[x + 1] = (sprite[i] & (1<<(width-1-j))) ? color[1] : 0x00;
                pixels_for_sprite_line[x + 2] = (sprite[i] & (1<<(width-1-j))) ? color[2] : 0x00;
            }
        }
        for (uint32_t y = 0; y < grid_dimension; y++) 
        {
            seek_hdmi(currentPoint+(NEW_LINE)*y);
            write_hdmi(pixels_for_sprite_line, width * grid_dimension * PIXEL_SIZE_GLOBAL);
        }
        currentPoint += NEW_LINE * grid_dimension;
        seek_hdmi(currentPoint);
    }
}

void toggle_alien_sprite(const uint32_t* sprite_val, int16_t x, int16_t y)
{
    if (sprite_val == alien_top_in_13x8)
    {
        alien_army_sprites[y][x] = alien_top_out_13x8;
    }
        
    else if (sprite_val == alien_top_out_13x8)
    {
        alien_army_sprites[y][x] = alien_top_in_13x8;
    }
        
    else if (sprite_val == alien_middle_in_13x8)
    {
        alien_army_sprites[y][x] = alien_middle_out_13x8;
    }
        
    else if (sprite_val == alien_middle_out_13x8)
    {
        alien_army_sprites[y][x] = alien_middle_in_13x8;
    }
        
    else if (sprite_val == alien_bottom_in_13x8)
    {
        alien_army_sprites[y][x] = alien_bottom_out_13x8;
    }
        
    else if (sprite_val == alien_bottom_out_13x8)
    {
        alien_army_sprites[y][x] = alien_bottom_in_13x8;
    }           
}

void toggle_all_sprites()
{
    for (uint16_t y = 0; y < 5; y++) 
    {
        for (uint16_t x = 0; x < 11; x++) 
        {
            // Change sprite
            toggle_alien_sprite(alien_army_sprites[y][x], x, y);
        }
    }

}

void move_alien_army() 
{
    int8_t move_distance;
    uint32_t old_pos = current_pos_alien;
    bool move_down;

    toggle_all_sprites();

    if ((current_pos_alien + (13 + SPACE_BW_ALIENS) * 2 * PIXEL_SIZE_GLOBAL * 11) % NEW_LINE == 0)
    {
        moving_right_alien = false;
        current_pos_alien += NEW_LINE * 15;
    }
    else if (current_pos_alien % NEW_LINE == 0)
    {
        moving_right_alien = true;
        current_pos_alien += NEW_LINE * 15;
    }
    
    move_distance = moving_right_alien ? (PIXEL_SIZE_GLOBAL * 2) : -(PIXEL_SIZE_GLOBAL * 2);
    move_down = (current_pos_alien != old_pos) ? true : false;
    old_pos = current_pos_alien;

    for (uint16_t alien_y = 0; alien_y < 5; alien_y++)                                                                  // for each row of aliens
    {
        for (uint32_t i = 0; i < 8; i++)                                                                           // For each row of the individual alien - 8
        {
            if (move_down)
            {   
                int16_t correction = (moving_right_alien) ? (NEW_LINE * 15) : (NEW_LINE * 15) - move_distance;
                draw_lots_o_aliens(current_pos_alien - correction, 13, i, alien_y, PIXEL_SIZE_GLOBAL * 2, black, true);
            }
            draw_lots_o_aliens(current_pos_alien, 13, i, alien_y, PIXEL_SIZE_GLOBAL * 2, magenta, false);
            current_pos_alien += NEW_LINE * 2;
            seek_hdmi(current_pos_alien);
        }
        current_pos_alien += NEW_LINE * 15;
        seek_hdmi(current_pos_alien);
    }
    current_pos_alien = old_pos + move_distance;
    seek_hdmi(current_pos_alien);
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
            return NULL;
    }
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

void update_lives(bool firstRun) 
{
    uint32_t pos = RIGHT_EDGE - LETTER_WIDTH * 9 + 640 * 3 * 2;
    for (uint8_t l = 0; l < lives; l++) {
        draw_alien(tank_15x8, pos, 15, 8, PIXEL_SIZE_GLOBAL, cyan);
        pos += 15 * PIXEL_SIZE_GLOBAL + 15;
    }
}

void move_saucer()
{
    if (saucer_moving)
    {
        if (saucer_pos % NEW_LINE != 1818)
        {
            // draw_alien(saucer_16x7, saucer_pos, 16, 7, PIXEL_SIZE_GLOBAL*2, black);
            draw_alien(block_2x8, saucer_pos, 2, 8, PIXEL_SIZE_GLOBAL*2, black);
            saucer_pos += PIXEL_SIZE_GLOBAL * 2;
            draw_alien(saucer_16x7, saucer_pos, 16, 7, PIXEL_SIZE_GLOBAL*2, green);
        }
        else
        {
            saucer_moving = false;
            draw_alien(saucer_16x7, saucer_pos, 16, 7, PIXEL_SIZE_GLOBAL*2, black);
            saucer_pos = SAUCER_STARTING_POS;
        }
    }
}

void blink_cursor(bool force) 
{
    draw_sprite(letterBLOCK_6x6, 
                LETTER_WIDTH*24 - 3 + (selected_char * LETTER_WIDTH) + 640*79*PIXEL_SIZE_GLOBAL, 
                6, 
                6, 
                PIXEL_SIZE_GLOBAL*2, 
                force ? black : 
                    blink ? cyan : black
    );
    draw_sprite(get_sprite_from_digit(*get_selected_char()),     
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

void main_print_score(score_t score, uint32_t pos)
{
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