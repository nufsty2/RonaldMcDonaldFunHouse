#include "draw_player.h"

extern uint32_t current_pos_bullet;
extern uint32_t current_pos_player;
extern bool bullet_moving;
extern char black[];
extern char cyan[];


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
    current_pos_bullet -= (NEW_LINE*4); // make bullet go up

    draw_alien(tankbullet_gone_1x5, old_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL*2, black); // erase old bullet

    draw_alien(tankbullet_1x5, current_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL*2, cyan); // draw new bullet

    // At this point, bullet gone
    if (current_pos_bullet < (640 + (NEW_LINE * 10))) 
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