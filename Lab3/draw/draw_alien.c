#include "draw_alien.h"
#include "../sprites/sprites.c"
#include "../draw/draw_ui.h"
#include "../draw/bunker.h"

#define SAUCER_HEIGHT 7
#define SACCER_WIDTH  16

#define BOTTOM_ROW 4

/* Positions */
extern uint32_t current_pos_saucer;
extern uint32_t current_pos_alien;
extern uint32_t current_pos_bullet;

/* Colors */
extern char black_pixels[];
extern char black[];
extern char green[];
extern char magenta[];

/* Flags */
extern bool moving_right_alien;
extern bool bullet_moving;
extern bool saucer_moving;

/* Edge Trackers */
uint16_t rightmost_col = NO_ALIEN_X - 1;
uint16_t leftmost_col = 0;

/* Alien army sprites stored locally because storing them globally is not good */
const uint32_t* alien_army_sprites[NO_ALIEN_Y][NO_ALIEN_X] = 
{
    {alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8,    alien_top_in_13x8   },
    {alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8},
    {alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8, alien_middle_in_13x8},
    {alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8},
    {alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8, alien_bottom_in_13x8}
};

/* bool array to see which one is still alive */
bool alien_army_is_alive[NO_ALIEN_Y][NO_ALIEN_X] = 
{
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true},
    {true, true, true, true, true, true, true, true, true, true, true}
};

/* Alien bullet oscillations */
const uint32_t* alien_bullet_sprites[MAX_BULLETS] =
{
    alienbullet2_up_3x5, alienbullet2_up_3x5, alienbullet2_up_3x5, alienbullet2_up_3x5
};
extern uint32_t alien_bullet_pos[];
extern bool alien_bullet_moving[];
uint8_t alien_fire_col = 0;

// This is one of our init function for the alien, it just sets the random variables
void draw_alien_init() 
{
    srand(time(0));
}

// This function will continously track the aliens on the right side
void draw_alien_track_right_edge() 
{
    uint16_t old_right_edge = rightmost_col;

    // For loop through the column
    for (int16_t col = old_right_edge; col >= 0; col--) 
    {
        for (uint16_t row = 0; row < NO_ALIEN_Y; row++) 
        {
            // Set new rightmost column
            if (alien_army_is_alive[row][col]) 
            {
                rightmost_col = col;
                return;
            }
        }
    }
}

// This function is the same above but it tracks aliens on the left side
void draw_alien_track_left_edge() 
{
    uint16_t old_left_edge = leftmost_col;

    // For loop through the column
    for (int16_t col = old_left_edge; col < NO_ALIEN_X; col++) 
    {
        for (uint16_t row = 0; row < NO_ALIEN_Y; row++) 
        {
            // Set left most column
            if (alien_army_is_alive[row][col]) 
            {
                leftmost_col = col;
                return;
            }
        }
    }
}

// This function toggles the  bulltes
// @ param bullet_num - bullet to toggle
void draw_alien_toggle_bullet_sprite(uint8_t bullet_num) 
{
    if (alien_bullet_sprites[bullet_num] == alienbullet2_up_3x5) 
    {
        alien_bullet_sprites[bullet_num] = alienbullet2_down_3x5;
    }
    else if (alien_bullet_sprites[bullet_num] == alienbullet2_down_3x5) 
    {
        alien_bullet_sprites[bullet_num] = alienbullet2_up_3x5;
    }
}

// This is our main function for drawing the whole alien army
void draw_lots_o_aliens(uint32_t pos, uint32_t width, uint32_t sprite_row, uint32_t alien_y, uint16_t pixel_size, char color[], bool erase_aliens)
{
    uint32_t currentPoint = pos; // get the pos and store it locally cuz we gonna be modifying the crap out of its
    uint32_t grid_dimension = pixel_size / PIXEL_SIZE_GLOBAL; // get the size scalar

    char pixels_for_entire_line[(width + SPACE_BW_ALIENS) * grid_dimension * PIXEL_SIZE_GLOBAL * NO_ALIEN_X]; // create a char array for the whole alien
    if (!erase_aliens) // if we ain't erasing
    {
        for (uint16_t alien_x = 0; alien_x < NO_ALIEN_X; alien_x++) // Loop through 5 X aliens                                                     
        {
            const uint32_t* sprite = alien_army_sprites[alien_y][alien_x]; // get the sprite from the array

            for (uint32_t j = (alien_x*(width+SPACE_BW_ALIENS)); j < (alien_x+1) * (width + SPACE_BW_ALIENS); j++)          
            {
                for (uint32_t x = j * PIXEL_SIZE_GLOBAL * grid_dimension; x < (j + 1) * grid_dimension * PIXEL_SIZE_GLOBAL; x+=PIXEL_SIZE_GLOBAL)
                {
                    // Set the pixels based on the maths we did above
                    pixels_for_entire_line[x + PIXEL_BYTE_0] = (((j >= alien_x*(width+SPACE_BW_ALIENS)) && (j < width*(alien_x+1)+(SPACE_BW_ALIENS*alien_x))) && (sprite[sprite_row] & (1<<(width-1-(j-(width+SPACE_BW_ALIENS)*alien_x))))) ? color[PIXEL_BYTE_0] : BLACK_PIXEL;
                    pixels_for_entire_line[x + PIXEL_BYTE_1] = (((j >= alien_x*(width+SPACE_BW_ALIENS)) && (j < width*(alien_x+1)+(SPACE_BW_ALIENS*alien_x))) && (sprite[sprite_row] & (1<<(width-1-(j-(width+SPACE_BW_ALIENS)*alien_x))))) ? color[PIXEL_BYTE_1] : BLACK_PIXEL;
                    pixels_for_entire_line[x + PIXEL_BYTE_2] = (((j >= alien_x*(width+SPACE_BW_ALIENS)) && (j < width*(alien_x+1)+(SPACE_BW_ALIENS*alien_x))) && (sprite[sprite_row] & (1<<(width-1-(j-(width+SPACE_BW_ALIENS)*alien_x))))) ? color[PIXEL_BYTE_2] : BLACK_PIXEL;
                }
            }
        }
    }

    // Draw the aliens based on grid dimension - size scalar
    for (uint32_t y = 0; y < grid_dimension; y++) 
    {        
        seek_hdmi(currentPoint+(NEW_LINE)*y); // get to right point
        write_hdmi((erase_aliens) ? black_pixels : pixels_for_entire_line, (width + SPACE_BW_ALIENS) * grid_dimension * PIXEL_SIZE_GLOBAL * NO_ALIEN_X); // draw
    }
}

// This is our main draw function, we use this to draw a lot of different things
void draw_alien(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size, char color[])
{
    // Get all points
    uint32_t initPoint = pos; 
    uint32_t currentPoint = initPoint;
    // This is a scalar
    uint32_t grid_dimension = pixel_size / PIXEL_SIZE_GLOBAL;

    for (uint32_t i = 0; i < height; i++) // loop through height of sprite
    {
        char pixels_for_sprite_line[width * grid_dimension * PIXEL_SIZE_GLOBAL]; // write each line
        for (uint32_t j = 0; j < width; j++) // loop through width of sprite
        {
            for (uint32_t x = j * PIXEL_SIZE_GLOBAL * grid_dimension; x < (j + 1) * grid_dimension * PIXEL_SIZE_GLOBAL; x+=PIXEL_SIZE_GLOBAL) 
            {
                // Set the pixels
                pixels_for_sprite_line[x + PIXEL_BYTE_0] = (sprite[i] & (1<<(width-1-j))) ? color[PIXEL_BYTE_0] : BLACK_PIXEL;
                pixels_for_sprite_line[x + PIXEL_BYTE_1] = (sprite[i] & (1<<(width-1-j))) ? color[PIXEL_BYTE_1] : BLACK_PIXEL;
                pixels_for_sprite_line[x + PIXEL_BYTE_2] = (sprite[i] & (1<<(width-1-j))) ? color[PIXEL_BYTE_2] : BLACK_PIXEL;
            }
        }
        for (uint32_t y = 0; y < grid_dimension; y++) // This for loop does the drawing based on the grid dimension, which is a size scalar
        {
            seek_hdmi(currentPoint+(NEW_LINE)*y); // seek to the right area
            write_hdmi(pixels_for_sprite_line, width * grid_dimension * PIXEL_SIZE_GLOBAL); // draw
        }
        currentPoint += NEW_LINE * grid_dimension; // new line
        seek_hdmi(currentPoint); // go to new line
    }
}

// This is our helper function to toggle each indivudal sprite, will be looped  through for all of them
// @ param sprite_val - this is the sprite to change
// @ param x - x position of the alien in the 2d array
// @ param y - y position of the alien in the 2d array
void toggle_alien_sprite(const uint32_t* sprite_val, int16_t x, int16_t y)
{

    if (sprite_val == alien_gone_13x8) // if gone, don't toggle at all
    {
        return; // do nothing
    }

    /* Else, toggle each one */
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

// This is a helper function to toggle the sprites in our move aliens function
void toggle_all_sprites()
{
    /* Loop through all the aliens */
    for (uint16_t y = 0; y < NO_ALIEN_Y; y++) 
    {
        for (uint16_t x = 0; x < NO_ALIEN_X; x++) 
        {
            // Change sprite
            toggle_alien_sprite(alien_army_sprites[y][x], x, y); // change the sprite each time
        }
    }
}

// This is our move saucer function, it will move the saucer every 30 seconds
// @param saucer_moving_local - this is what we pass in to the move saucer function to see if moving
bool move_saucer(bool saucer_moving_local)
{
    if (saucer_moving_local) // pass in variable to se if it is moving
    {
        if (current_pos_saucer % NEW_LINE != FAR_RIGHT_BOUNDRY_FOR_SAUCER) // if it didn't hit the far right boundry
        {
            draw_alien(block_2x8, current_pos_saucer, BLOCK_WIDTH, BLOCK_HEIGHT, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black); // draw
            current_pos_saucer += PIXEL_SIZE_GLOBAL * SIZE_SCALAR; // inc the pos
            draw_alien(saucer_16x7, current_pos_saucer, SAUCER_WIDTH, SAUCER_HEIGHT, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, green); // draw

            return true; // return true, still moving
        }
        else // if it hit edge of screen
        {
            draw_alien(saucer_16x7, current_pos_saucer, SAUCER_WIDTH, SAUCER_HEIGHT, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black); // erase
            current_pos_saucer = SAUCER_STARTING_POS; // reset saucer pos
            
            return false; // return false, not moving anymore
        }
    }
}

// This is our move alien army function, this gets called repeatly because aliens are moving all the time
void move_alien_army() 
{
    int8_t move_distance; // move disstance
    uint32_t old_pos = current_pos_alien; // calculate old pos to erase
    bool move_down; // bool to determine if we should move down

    toggle_all_sprites();

    /* If an alien has hit the right edge */
    if ((current_pos_alien + (SPACE_MOVING_ALIENS + SPACE_BW_ALIENS) * SIZE_SCALAR * PIXEL_SIZE_GLOBAL * (rightmost_col + 1)) % NEW_LINE == 0)
    {
        moving_right_alien = false; // now move left
        current_pos_alien += NEW_LINE * MOVE_ROWS_DOWN_FOR_ALIENS; // move aliens down
    }
    /* If an alien has hit the left edge */
    else if ((current_pos_alien + (SPACE_MOVING_ALIENS + SPACE_BW_ALIENS) * SIZE_SCALAR * PIXEL_SIZE_GLOBAL * leftmost_col) % NEW_LINE == 0)
    {
        moving_right_alien = true; // move back right
        current_pos_alien += NEW_LINE * MOVE_ROWS_DOWN_FOR_ALIENS; // move down
    }
    
    move_distance = moving_right_alien ? (PIXEL_SIZE_GLOBAL * SIZE_SCALAR) : -(PIXEL_SIZE_GLOBAL * SIZE_SCALAR); // calculate move distance
    move_down = (current_pos_alien != old_pos) ? true : false; // determine to move down
    old_pos = current_pos_alien; // set old position

    for (uint16_t alien_y = 0; alien_y < NO_ALIEN_Y; alien_y++) // loop through aliens at Y
    {  
        for (uint32_t height = 0; height < 8; height++) // loop through height of the alien                                                                          
        {
            if (move_down) // if we are moving down, draw down
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

// Debugg print statement
void draw_alien_debug_print() 
{
    printf("starting_pos_alien_y: %d\n\r", draw_alien_get_y_coord(current_pos_alien));
    printf("starting_pos_alien_x: %d\n\n\r", draw_alien_get_x_coord(current_pos_alien, draw_alien_get_y_coord(current_pos_alien)));
}

// This function will be called when a bullet is fired and consistently check if an alien has been hit
bool draw_alien_detect_hit_army()
{
    for (int16_t row = NO_ALIEN_Y - 1; row >= 0; row--) // loop through the 5 Y aliens
    {
        // Set y borders
        uint32_t top_border = current_pos_alien + NEW_LINE * (row * (ALIEN_SPRITE_HEIGHT * SIZE_SCALAR + MOVE_ROWS_DOWN_FOR_ALIENS));
        uint32_t bot_border = current_pos_alien + NEW_LINE * ((row + 1) * ALIEN_SPRITE_HEIGHT * SIZE_SCALAR + row * MOVE_ROWS_DOWN_FOR_ALIENS);

        // Get coord values for the Y borders
        uint16_t top_border_y = draw_alien_get_y_coord(top_border);
        uint16_t bot_border_y = draw_alien_get_y_coord(bot_border);

        
        for (int16_t col = NO_ALIEN_X - 1; col >= 0; col--) // loop through the 11 X aliens
        {  
            // Set x borders
            uint32_t left_border = current_pos_alien + PIXEL_SIZE_GLOBAL * SIZE_SCALAR * (col * (ALIEN_SPRITE_WIDTH + SPACE_BW_ALIENS));
            uint32_t right_border = current_pos_alien + PIXEL_SIZE_GLOBAL * SIZE_SCALAR * ((col + 1) * ALIEN_SPRITE_WIDTH + col * SPACE_BW_ALIENS);

            // Get the coord values based of all of those
            uint16_t bullet_y = draw_alien_get_y_coord(current_pos_bullet);
            uint16_t bullet_x = draw_alien_get_x_coord(current_pos_bullet, bullet_y);
            uint16_t left_border_y = draw_alien_get_y_coord(left_border);
            uint16_t right_border_y = draw_alien_get_y_coord(right_border);
            uint16_t left_border_x = draw_alien_get_x_coord(left_border, left_border_y);
            uint16_t right_border_x = draw_alien_get_x_coord(right_border, right_border_y);       

            // Check if a bullet is within one of the alien boxes
            if ((bullet_x >= left_border_x) &&
                (bullet_x <  right_border_x) &&
                (bullet_y >= top_border_y) &&
                (bullet_y <  bot_border_y))
            {
                
                // Check to see if alien already dead, if so, GTFO
                if (!(alien_army_is_alive[row][col])) { return false; }  

                // Change sprite to the explosion and set boolean flag
                alien_army_is_alive[row][col] = false;
                alien_army_sprites[row][col] = alien_explosion_12x10;

                // Re-track edges
                draw_alien_track_right_edge();
                draw_alien_track_left_edge();

                // Update score
                draw_ui_increase_score(row);
                update_score(false);

                bullet_moving = false; // make sure bullet stops travelling
                draw_alien(tankbullet_1x5, current_pos_bullet, PLAYER_BULLET_WIDTH, PLAYER_BULLET_HEIGHT, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black); // erase bullet

                draw_alien_check_alien_reset();

                return true; // true = alien hit
            }
        }
    }

    return false; // hit here if we got nothing at all
}

// This function detects a hit on the saucer
void draw_alien_detect_hit_saucer() 
{
    // Set borderstop_left_border_saucertop_left_border_saucer
    uint32_t top_left_border_saucer = current_pos_saucer;
    uint32_t bot_right_border_saucer = current_pos_saucer + SIZE_SCALAR * (NEW_LINE * SAUCER_HEIGHT + PIXEL_SIZE_GLOBAL * SAUCER_WIDTH);

    // Get coord values for the Y borders
    uint16_t top_border_saucer_y = draw_alien_get_y_coord(top_left_border_saucer);
    uint16_t bot_border_saucer_y = draw_alien_get_y_coord(bot_right_border_saucer);
    uint16_t left_border_saucer_x = draw_alien_get_x_coord(top_left_border_saucer, top_border_saucer_y);
    uint16_t right_border_saucer_x = draw_alien_get_x_coord(bot_right_border_saucer, bot_border_saucer_y);       

    // Get the coord values for bullet
    uint16_t bullet_y = draw_alien_get_y_coord(current_pos_bullet);
    uint16_t bullet_x = draw_alien_get_x_coord(current_pos_bullet, bullet_y);

    // Check if a bullet is within one of the saucer boxes
    if ((bullet_x >= left_border_saucer_x) &&
        (bullet_x <  right_border_saucer_x) &&
        (bullet_y >= top_border_saucer_y) &&
        (bullet_y <  bot_border_saucer_y))
    {
        // Kill the saucer
        saucer_moving = false;
        bullet_moving = false;
        draw_alien(saucer_16x7, current_pos_saucer, SAUCER_WIDTH, SAUCER_HEIGHT, PIXEL_SIZE_GLOBAL*SIZE_SCALAR, black); // erase
        draw_alien(tankbullet_1x5, current_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL*2, black); // erase old bullet
        current_pos_saucer = SAUCER_STARTING_POS; // reset saucer pos

        // Inc the score
        draw_ui_increase_score_saucer();
        update_score(false);
    }
}

// This function makes sure all aliens are dead
void draw_alien_check_alien_reset() 
{
    // Loop through all aliens
    for (uint16_t row = 0; row < NO_ALIEN_Y; row++)
    {
        for (uint16_t col = 0; col < NO_ALIEN_X; col++) 
        {
            // If alive, get outta there
            if (alien_army_is_alive[row][col]) 
            {
                return;
            }
        }
    }

    // If we got here, they're all dead
    // Reset aliens
    erase_dead_aliens();
    // Wait until explosion is over
    move_alien_army();

    current_pos_alien = ALIEN_START_POS;
    moving_right_alien = true;
    rightmost_col = NO_ALIEN_X - 1;
    leftmost_col = 0;
    for (uint16_t row = 0; row < NO_ALIEN_Y; row++)
    {
        for (uint16_t col = 0; col < NO_ALIEN_X; col++) {
            // Reset booleans
            alien_army_is_alive[row][col] = true;
            // Reset sprites
            switch (row) {
                case TOP_ALIEN:
                    alien_army_sprites[row][col] = alien_top_in_13x8;
                    break;
                case MID_ALIEN_0:
                case MID_ALIEN_1:
                    alien_army_sprites[row][col] = alien_middle_in_13x8;
                    break;
                case BOT_ALIEN_0:
                case BOT_ALIEN_1:
                    alien_army_sprites[row][col] = alien_bottom_in_13x8;
                    break;
            }
        }
    }
    draw_ui_increment_lives();
}

// This function checks to see if an alien is dead by checking the bool array, if it is, erase
void erase_dead_aliens()
{
    for (uint16_t y = 0; y < NO_ALIEN_Y; y++) // loop through the 5 Y aliens
    {
        for (uint16_t x = 0; x < NO_ALIEN_X; x++) // loop through the 11 X aliens
        {
            // Check boolean table
            if (!(alien_army_is_alive[y][x])) // if dead
            {
                alien_army_sprites[y][x] = alien_gone_13x8; // replace with alien gone (just a black box)
            }
        }
    } 
}

// This function fires the alien bullet
void draw_alien_fire_alien_bullet(uint8_t bullet_num)
{
    // Declare variables
    uint32_t old_pos_bullet = alien_bullet_pos[bullet_num];

    uint16_t y_coord = draw_alien_get_y_coord(old_pos_bullet);

    alien_bullet_pos[bullet_num] += (NEW_LINE*2); // make bullet go down

    draw_alien(alien_bullet_sprites[bullet_num], old_pos_bullet, 3, 5, PIXEL_SIZE_GLOBAL*2, black); // erase old bullet
    draw_alien_toggle_bullet_sprite(bullet_num);

    draw_alien(alien_bullet_sprites[bullet_num], alien_bullet_pos[bullet_num], 3, 5, PIXEL_SIZE_GLOBAL*2, magenta); // draw new bullet

    // At this point, bullet gone
    if (alien_bullet_pos[bullet_num] > (NEW_LINE * 470)) 
    {
        alien_bullet_moving[bullet_num] = false;
        draw_alien(alien_bullet_sprites[bullet_num], alien_bullet_pos[bullet_num], 3, 5, PIXEL_SIZE_GLOBAL*2, black); // erase old bullet
    }
}

// This function loops through the aliens and fires the bullets
void draw_alien_fire_alien_bullets() 
{
    for (uint8_t i = 0; i < MAX_BULLETS; i++) 
    {
        if (alien_bullet_moving[i]) 
        {
            draw_alien_fire_alien_bullet(i);
        }
    }
}

// This function will draw the alien bullets, should be a max of 4 at a time
void draw_alien_trigger_bullets()
{
    // SEt random variables for triggering bullets
    uint8_t random = rand() % 3 + 1;
    bool increment = rand() % 2;
    
    if (increment) // if inc was a good value
    {
        if (alien_fire_col + random > 10) 
        {
            alien_fire_col = 0;
        }
        else 
        {
            alien_fire_col += random;
        }
    }
    else 
    {
        if (alien_fire_col - random < 0) 
        {
            alien_fire_col = 10;
        }
        else 
        {
            alien_fire_col -= random;
        }
    }

    int8_t alien_fire_row = BOTTOM_ROW; // make sure it's bottom row

    while(!alien_army_is_alive[alien_fire_row][alien_fire_col]) // if an alien dead on the bottom row, move to next one
    {
        alien_fire_row--;
    }
    if (alien_fire_row < 0) 
    {
        return;
    }
    
    uint8_t bullet_available;
    for (bullet_available = 0; bullet_available < MAX_BULLETS; bullet_available++) 
    {
        if (!alien_bullet_moving[bullet_available]) // cehck to see if the  bullet is available (not moving)
        {
            break;
        }
    }

    if (bullet_available < MAX_BULLETS) 
    {
        alien_bullet_pos[bullet_available] = current_pos_alien + 
                                            // Vertical
                                            NEW_LINE * ((alien_fire_row + 1) * ALIEN_SPRITE_HEIGHT * SIZE_SCALAR + alien_fire_row * MOVE_ROWS_DOWN_FOR_ALIENS) +
                                            // Horizontal
                                            PIXEL_SIZE_GLOBAL * (alien_fire_col * SIZE_SCALAR * (ALIEN_SPRITE_WIDTH + SPACE_BW_ALIENS) + ALIEN_SPRITE_WIDTH);
        alien_bullet_moving[bullet_available] = true;
        draw_alien_fire_alien_bullet(bullet_available);
    }
}
