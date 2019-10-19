#include "player.h"
#include "ui.h"
#include "../sprites/sprites.c"

/* Player attributes */
extern uint32_t current_pos_bullet;
extern uint32_t current_pos_player;
extern bool bullet_moving;
extern bool player_dead;

/* Colors */
extern char black[];
extern char cyan[];

/* Button value */
extern uint32_t buttons_val;

/* Alien attributes */
extern uint32_t alien_bullet_pos[];
extern bool alien_bullet_moving[];

// This function fires a bullet
void player_fire_bullet()
{
    // Declare variables
    uint32_t old_pos_bullet = current_pos_bullet;

    uint16_t y_coord = draw_alien_get_y_coord(current_pos_bullet);
    
    // Move bullet up
    old_pos_bullet = current_pos_bullet;
    current_pos_bullet -= (NEW_LINE*8); // make bullet go up

    alien_draw(tankbullet_gone_1x5, old_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL*2, black); // erase old bullet

    alien_draw(tankbullet_1x5, current_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL*2, cyan); // draw new bullet

    // At this point, bullet gone
    if (current_pos_bullet < (NEW_LINE * 15)) 
    {
        bullet_moving = false;
        alien_draw(tankbullet_1x5, current_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL*2, black); // erase old bullet
    }
}

// This function moves the player right and make sure he doesn't loop over the screen
void move_player_right()
{
    if ((current_pos_player+15) % NEW_LINE != 1839)
    {
        alien_draw(block_2x8, current_pos_player, 2, 8, PIXEL_SIZE_GLOBAL*2, black);

        current_pos_player += PIXEL_SIZE_GLOBAL * 4;

        alien_draw(tank_15x8, current_pos_player, 15, 8, PIXEL_SIZE_GLOBAL*2, cyan);
    }
}

// This function moves the player left and make sure he doesn't loop over the screen
void move_player_left()
{
    if (current_pos_player % NEW_LINE != 0)
    {
        alien_draw(block_2x8, current_pos_player+13*6, 2, 8, PIXEL_SIZE_GLOBAL*2, black);

        current_pos_player -= PIXEL_SIZE_GLOBAL * 4;

        alien_draw(tank_15x8, current_pos_player, 15, 8, PIXEL_SIZE_GLOBAL*2, cyan);
    }
}

// This function combines the two above
void player_move()
{
    if (buttons_val == BTN_0)
        move_player_right();
    else if (buttons_val == BTN_1)
        move_player_left();
}

// This function destroys the player on alien bullet hit
void destroy_player()
{
    player_dead = true;
    ui_decrement_lives(); // magic
}

// This function detects alien bullet hits on the player
void player_detect_alien_hit() 
{
    for (uint8_t i = 0; i < MAX_BULLETS; i++)
    {
        uint32_t bullet_bottom_left = alien_bullet_pos[i] + SIZE_SCALAR * NEW_LINE * 5;
        uint32_t bullet_bottom_right = bullet_bottom_left + SIZE_SCALAR * PIXEL_SIZE_GLOBAL * 3;

        // Set x borders
        uint32_t left_border = current_pos_player;
        uint32_t right_border = current_pos_player + 15 * PIXEL_SIZE_GLOBAL * SIZE_SCALAR;

        // Set y borders
        uint32_t top_border = current_pos_player;
        uint32_t bot_border = current_pos_player + 8 * NEW_LINE * SIZE_SCALAR;

        // Get coord values for the Y borders
        uint16_t top_border_y = draw_alien_get_y_coord(top_border);
        uint16_t bot_border_y = draw_alien_get_y_coord(bot_border);

        // Get the coord values based of all of those
        uint16_t bullet_bottom_y = draw_alien_get_y_coord(bullet_bottom_left);
        uint16_t bullet_bottom_left_x = draw_alien_get_x_coord(bullet_bottom_left, bullet_bottom_y);
        uint16_t bullet_bottom_right_x = draw_alien_get_x_coord(bullet_bottom_right, bullet_bottom_y);
        uint16_t left_border_y = draw_alien_get_y_coord(left_border);
        uint16_t right_border_y = draw_alien_get_y_coord(right_border);
        uint16_t left_border_x = draw_alien_get_x_coord(left_border, left_border_y);
        uint16_t right_border_x = draw_alien_get_x_coord(right_border, right_border_y);       

        // Check if a bullet is within one of the bunker blocks
        if ((bullet_bottom_left_x >= left_border_x  || bullet_bottom_right_x >= left_border_x)  &&
            (bullet_bottom_left_x <  right_border_x || bullet_bottom_right_x <  right_border_x) &&
            (bullet_bottom_y >= top_border_y) &&
            (bullet_bottom_y <  bot_border_y))
        {
            // Erase bullet
            alien_bullet_moving[i] = false; // make sure bullet stops travelling
            alien_draw(alienbullet2_down_3x5, alien_bullet_pos[i], 3, 5, PIXEL_SIZE_GLOBAL * SIZE_SCALAR, black); // erase bullet
            destroy_player();
            return;
        }
    }
}