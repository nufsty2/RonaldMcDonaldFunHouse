#include "draw_player.h"
#include "draw_ui.h"

extern uint32_t current_pos_bullet;
extern uint32_t current_pos_player;
extern bool bullet_moving;
extern char black[];
extern char cyan[];
extern uint32_t buttons_val;
extern uint32_t alien_bullet_pos[];
extern bool alien_bullet_moving[];
extern bool player_dead;


void fire_bullet()
{
    // Declare variables
    uint32_t old_pos_bullet = current_pos_bullet;

    uint16_t y_coord = draw_alien_get_y_coord(current_pos_bullet);
    //printf("Bullet coord = (%d, %d)\n\r", draw_alien_get_x_coord(current_pos_bullet, y_coord), y_coord);

    // Draw the bullet
    draw_alien(tankbullet_1x5, current_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL*2, cyan); // draw the bullet

    // Move bullet up
    old_pos_bullet = current_pos_bullet;
    current_pos_bullet -= (NEW_LINE*8); // make bullet go up

    draw_alien(tankbullet_gone_1x5, old_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL*2, black); // erase old bullet

    draw_alien(tankbullet_1x5, current_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL*2, cyan); // draw new bullet

    // At this point, bullet gone
    if (current_pos_bullet < (NEW_LINE * 15)) 
    {
        bullet_moving = false;
        draw_alien(tankbullet_1x5, current_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL*2, black); // erase old bullet
    }
}

void move_player_right()
{
    if ((current_pos_player+15) % NEW_LINE != 1839)
    {
        draw_alien(block_2x8, current_pos_player, 2, 8, PIXEL_SIZE_GLOBAL*2, black);

        current_pos_player += PIXEL_SIZE_GLOBAL * 4;

        draw_alien(tank_15x8, current_pos_player, 15, 8, PIXEL_SIZE_GLOBAL*2, cyan);
    }
}

void move_player_left()
{
    if (current_pos_player % NEW_LINE != 0)
    {
        draw_alien(block_2x8, current_pos_player+13*6, 2, 8, PIXEL_SIZE_GLOBAL*2, black);

        current_pos_player -= PIXEL_SIZE_GLOBAL * 4;

        draw_alien(tank_15x8, current_pos_player, 15, 8, PIXEL_SIZE_GLOBAL*2, cyan);
    }
}

void move_player()
{
    if (buttons_val == BTN_0)
        move_player_right();
    else if (buttons_val == BTN_1)
        move_player_left();
}

void destroy_player()
{
    player_dead = true;
    draw_ui_decrement_lives(); // magic
}

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

            // printf("Bullet: (%d, %d)\n\r", bullet_x, bullet_y);
            // printf("%d <= X < %d\n\r", left_border_x, right_border_x);
            // printf("%d <= Y < %d\n\n\r", top_border_y, bot_border_y);

            // Erase bullet
            alien_bullet_moving[i] = false; // make sure bullet stops travelling
            draw_alien(alienbullet2_down_3x5, alien_bullet_pos[i], 3, 5, PIXEL_SIZE_GLOBAL * SIZE_SCALAR, black); // erase bullet
            destroy_player();
            return;
        }
    }
}