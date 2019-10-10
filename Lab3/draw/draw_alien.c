#include "../globals/globals.h"


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