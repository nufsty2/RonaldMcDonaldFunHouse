#include "draw_alien.h"

extern const uint32_t* alien_army_sprites[][11];
extern uint32_t saucer_pos;
extern char black_pixels[];
extern char black[];
extern char green[];
extern char magenta[];
extern bool moving_right_alien;
extern uint32_t current_pos_alien;
extern uint32_t current_pos_bullet;
extern bool alien_army_is_alive[][11];
extern bool bullet_moving;

const uint32_t* alien_army_sprites[5][11] = 
{
    {alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8   },
    {alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8},
    {alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8},
    {alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8},
    {alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8}
};

/* bool array to see which one is still alive */
bool alien_army_is_alive[5][11] = 
{
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true}
};

void draw_lots_o_aliens(uint32_t pos, uint32_t width, uint32_t sprite_row, uint32_t alien_y, uint16_t pixel_size, char color[], bool erase_aliens)
{
    uint32_t currentPoint = pos;
    uint32_t grid_dimension = pixel_size / PIXEL_SIZE_GLOBAL;

    char pixels_for_entire_line[(width + SPACE_BW_ALIENS) * grid_dimension * PIXEL_SIZE_GLOBAL * NO_ALIEN_X]; 
    if (!erase_aliens)
    {
        for (uint16_t alien_x = 0; alien_x < NO_ALIEN_X; alien_x++)                                                        
        {
            const uint32_t* sprite = alien_army_sprites[alien_y][alien_x];

            for (uint32_t j = (alien_x*(width+SPACE_BW_ALIENS)); j < (alien_x+1) * (width + SPACE_BW_ALIENS); j++)          
            {
                for (uint32_t x = j * PIXEL_SIZE_GLOBAL * grid_dimension; x < (j + 1) * grid_dimension * PIXEL_SIZE_GLOBAL; x+=PIXEL_SIZE_GLOBAL)
                {
                    pixels_for_entire_line[x + PIXEL_BYTE_0] = (((j >= alien_x*(width+SPACE_BW_ALIENS)) && (j < width*(alien_x+1)+(SPACE_BW_ALIENS*alien_x))) && (sprite[sprite_row] & (1<<(width-1-(j-(width+SPACE_BW_ALIENS)*alien_x))))) ? color[PIXEL_BYTE_0] : BLACK_PIXEL;
                    pixels_for_entire_line[x + PIXEL_BYTE_1] = (((j >= alien_x*(width+SPACE_BW_ALIENS)) && (j < width*(alien_x+1)+(SPACE_BW_ALIENS*alien_x))) && (sprite[sprite_row] & (1<<(width-1-(j-(width+SPACE_BW_ALIENS)*alien_x))))) ? color[PIXEL_BYTE_1] : BLACK_PIXEL;
                    pixels_for_entire_line[x + PIXEL_BYTE_2] = (((j >= alien_x*(width+SPACE_BW_ALIENS)) && (j < width*(alien_x+1)+(SPACE_BW_ALIENS*alien_x))) && (sprite[sprite_row] & (1<<(width-1-(j-(width+SPACE_BW_ALIENS)*alien_x))))) ? color[PIXEL_BYTE_2] : BLACK_PIXEL;
                }
            }
        }
    }

    for (uint32_t y = 0; y < grid_dimension; y++) 
    {        
        seek_hdmi(currentPoint+(NEW_LINE)*y);
        write_hdmi((erase_aliens) ? black_pixels : pixels_for_entire_line, (width + SPACE_BW_ALIENS) * grid_dimension * PIXEL_SIZE_GLOBAL * NO_ALIEN_X);
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
                pixels_for_sprite_line[x + PIXEL_BYTE_0] = (sprite[i] & (1<<(width-1-j))) ? color[0] : BLACK_PIXEL;
                pixels_for_sprite_line[x + PIXEL_BYTE_1] = (sprite[i] & (1<<(width-1-j))) ? color[1] : BLACK_PIXEL;
                pixels_for_sprite_line[x + PIXEL_BYTE_2] = (sprite[i] & (1<<(width-1-j))) ? color[2] : BLACK_PIXEL;
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

    if (sprite_val == alien_gone_13x8) 
    {
        return;
    }

    else if (sprite_val == alien_top_in_13x8)
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
    for (uint16_t y = 0; y < NO_ALIEN_Y; y++) 
    {
        for (uint16_t x = 0; x < NO_ALIEN_X; x++) 
        {
            // Change sprite
            toggle_alien_sprite(alien_army_sprites[y][x], x, y);
        }
    }
}

bool move_saucer(bool saucer_moving_local)
{
    if (saucer_moving_local)
    {
        if (saucer_pos % NEW_LINE != FAR_RIGHT_BOUNDRY_FOR_SAUCER)
        {
            draw_alien(block_2x8, saucer_pos, BLOCK_WIDTH, BLOCK_HEIGHT, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
            saucer_pos += PIXEL_SIZE_GLOBAL * SIZE_SCALAR;
            draw_alien(saucer_16x7, saucer_pos, SAUCER_WIDTH, SAUCER_HEIGHT, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, green);

            return true;
        }
        else
        {
            draw_alien(saucer_16x7, saucer_pos, SAUCER_WIDTH, SAUCER_HEIGHT, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black);
            saucer_pos = SAUCER_STARTING_POS;
            
            return false;
        }
    }
}

void move_alien_army() 
{
    int8_t move_distance;
    uint32_t old_pos = current_pos_alien;
    bool move_down;

    toggle_all_sprites();

    /* If an alien has reached the end */
    if ((current_pos_alien + (SPACE_MOVING_ALIENS + SPACE_BW_ALIENS) * SIZE_SCALAR * PIXEL_SIZE_GLOBAL * NO_ALIEN_X) % NEW_LINE == 0)
    {
        moving_right_alien = false;
        current_pos_alien += NEW_LINE * MOVE_ROWS_DOWN_FOR_ALIENS;
    }
    else if (current_pos_alien % NEW_LINE == 0)
    {
        moving_right_alien = true;
        current_pos_alien += NEW_LINE * MOVE_ROWS_DOWN_FOR_ALIENS;
    }
    
    move_distance = moving_right_alien ? (PIXEL_SIZE_GLOBAL * SIZE_SCALAR) : -(PIXEL_SIZE_GLOBAL * SIZE_SCALAR);
    move_down = (current_pos_alien != old_pos) ? true : false;
    old_pos = current_pos_alien;

    for (uint16_t alien_y = 0; alien_y < NO_ALIEN_Y; alien_y++)
    {  
        for (uint32_t height = 0; height < 8; height++)                                                                          
        {
            if (move_down)
            {   
                int16_t correction = (moving_right_alien) ? (NEW_LINE * MOVE_ROWS_DOWN_FOR_ALIENS) : (NEW_LINE * MOVE_ROWS_DOWN_FOR_ALIENS) - move_distance;
                draw_lots_o_aliens(current_pos_alien - correction, ALIEN_SPRITE_WIDTH, height, alien_y, PIXEL_SIZE_GLOBAL * SIZE_SCALAR, black, true);
            }
            draw_lots_o_aliens(current_pos_alien, ALIEN_SPRITE_WIDTH, height, alien_y, PIXEL_SIZE_GLOBAL * SIZE_SCALAR, magenta, false);
            current_pos_alien += NEW_LINE * SIZE_SCALAR;
            seek_hdmi(current_pos_alien);
        }
        current_pos_alien += NEW_LINE * MOVE_ROWS_DOWN_FOR_ALIENS;
        seek_hdmi(current_pos_alien);
    }
    current_pos_alien = old_pos + move_distance;
    seek_hdmi(current_pos_alien);
}

bool draw_alien_detect_hit()
{

    for (uint32_t row = 0; row < NO_ALIEN_Y; row++)
    {
        // y positions
        uint32_t top_border = current_pos_alien + NEW_LINE * (row * (ALIEN_SPRITE_HEIGHT + MOVE_ROWS_DOWN_FOR_ALIENS));
        uint32_t bot_border = current_pos_alien + NEW_LINE * ((row + 1) * ALIEN_SPRITE_HEIGHT + row * MOVE_ROWS_DOWN_FOR_ALIENS);

        // coord_values
        uint16_t top_border_y = draw_alien_get_y_coord(top_border);
        uint16_t bot_border_y = draw_alien_get_y_coord(bot_border);


        for (uint32_t col = 0; col < NO_ALIEN_X; col++)
        {  
            // x positions
            uint32_t left_border = current_pos_alien + PIXEL_SIZE_GLOBAL * SIZE_SCALAR * (col * (ALIEN_SPRITE_WIDTH + SPACE_BW_ALIENS));
            uint32_t right_border = current_pos_alien + PIXEL_SIZE_GLOBAL * SIZE_SCALAR * ((col + 1) * ALIEN_SPRITE_WIDTH + col * SPACE_BW_ALIENS);

            // coord values
            uint16_t bullet_y = draw_alien_get_y_coord(current_pos_bullet);
            uint16_t bullet_x = draw_alien_get_x_coord(current_pos_bullet, bullet_y);
            uint16_t left_border_y = draw_alien_get_y_coord(left_border);
            uint16_t right_border_y = draw_alien_get_y_coord(right_border);
            uint16_t left_border_x = draw_alien_get_x_coord(left_border, left_border_y);
            uint16_t right_border_x = draw_alien_get_x_coord(right_border, right_border_y);            

            // printf("%d <= X < %d)\n\r", left_border_x, right_border_x);
            // printf("%d <= Y < %d)\n\r", top_border_y, bot_border_y);
            if ((bullet_x >= left_border_x) &&
                (bullet_x <  right_border_x) &&
                (bullet_y >= top_border_y) &&
                (bullet_y <  bot_border_y))
            {
                
                // Check to see if alien already dead, if so, GTFO
                if (!(alien_army_is_alive[row][col])) { return false; }  

                // Hit! Make an explosion
                // printf("HIT!!!!!!!!!! ALIEN[%d][%d]\n\r", row, col);

                // Change sprite to empty and set boolean flag
                alien_army_is_alive[row][col] = false;
                alien_army_sprites[row][col] = alien_explosion_12x10;


                bullet_moving = false;
                draw_alien(tankbullet_1x5, current_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL*2, black); // erase bullet
                return true;
            }
        }
    }

    return false;
}

void erase_dead_aliens()
{
    for (uint16_t y = 0; y < NO_ALIEN_Y; y++) 
    {
        for (uint16_t x = 0; x < NO_ALIEN_X; x++) 
        {
            // Check boolean table
            if (!(alien_army_is_alive[y][x]))
            {
                alien_army_sprites[y][x] = alien_gone_13x8;
            }
        }
    } 
}