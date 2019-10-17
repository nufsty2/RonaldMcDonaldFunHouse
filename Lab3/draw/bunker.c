#include "bunker.h"
#include "../sprites/sprites.c"
#include "../draw/draw_alien.h"

#define NUM_BUNKERS 4
#define NUM_BLOCKS  10

/* Externs */
extern uint32_t current_pos_bullet;
extern bool bullet_moving;
extern char tan[];
extern char black[];

extern uint32_t alien_bullet_pos[];
extern bool alien_bullet_moving[];


/* Bunkers */
uint32_t bunker_pos[NUM_BUNKERS][NUM_BLOCKS];
const uint32_t* bunker_sprites[NUM_BUNKERS][NUM_BLOCKS] = 
{
    {bunker_upper_left_gone_6x6, bunkerBlock_6x6, bunkerBlock_6x6, bunker_upper_right_gone_6x6, bunkerBlock_6x6, bunker_lower_left_gone_6x6, bunker_lower_right_gone_6x6, bunkerBlock_6x6, bunkerBlock_6x6, bunkerBlock_6x6},
    {bunker_upper_left_gone_6x6, bunkerBlock_6x6, bunkerBlock_6x6, bunker_upper_right_gone_6x6, bunkerBlock_6x6, bunker_lower_left_gone_6x6, bunker_lower_right_gone_6x6, bunkerBlock_6x6, bunkerBlock_6x6, bunkerBlock_6x6},
    {bunker_upper_left_gone_6x6, bunkerBlock_6x6, bunkerBlock_6x6, bunker_upper_right_gone_6x6, bunkerBlock_6x6, bunker_lower_left_gone_6x6, bunker_lower_right_gone_6x6, bunkerBlock_6x6, bunkerBlock_6x6, bunkerBlock_6x6},
    {bunker_upper_left_gone_6x6, bunkerBlock_6x6, bunkerBlock_6x6, bunker_upper_right_gone_6x6, bunkerBlock_6x6, bunker_lower_left_gone_6x6, bunker_lower_right_gone_6x6, bunkerBlock_6x6, bunkerBlock_6x6, bunkerBlock_6x6}
};

const uint32_t* bunker_damage[NUM_BUNKERS][NUM_BLOCKS] =
{
    {bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6},
    {bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6},
    {bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6},
    {bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6, bunkerDamage3_6x6}
};

const uint32_t* bunker_take_damage(const uint32_t* damage_sprite) {
    if (damage_sprite == bunkerDamage3_6x6)
    {
        return bunkerDamage2_6x6;
    }
    else if (damage_sprite == bunkerDamage2_6x6)
    {
        return bunkerDamage1_6x6;
    }
    else if (damage_sprite == bunkerDamage1_6x6)
    {
        return bunkerDamage0_6x6;
    }
    else if (damage_sprite == bunkerDamage0_6x6)
    {
        return bunkerGone_6x6;
    }
}

void bunker_init()
{
    uint32_t pos = PIXEL_SIZE_GLOBAL*(640*400 + 75);
    // Print bunkers
    for (uint8_t i = 0; i < NUM_BUNKERS; i++) 
    {
        uint32_t current_bunker_pos = pos;
        for (uint8_t j = 0; j < NUM_BLOCKS; j++)
        {
            bunker_pos[i][j] = pos; // assign bunkers
            if (j < 4)
            {
                pos = (j == 3) ? (current_bunker_pos + NEW_LINE * SIZE_SCALAR * 6) : (pos + PIXEL_SIZE_GLOBAL * SIZE_SCALAR * 6);
            }

            else if (j < 8)
            {
                pos = (j == 7) ? (current_bunker_pos + NEW_LINE * SIZE_SCALAR * 12) : (pos + PIXEL_SIZE_GLOBAL * SIZE_SCALAR * 6);
            }

            else if (j == 8)
            {
                pos += PIXEL_SIZE_GLOBAL * SIZE_SCALAR * 6 * 3;
            }
        }
        draw_alien(bunker_24x18, current_bunker_pos, 24, 18, PIXEL_SIZE_GLOBAL*2, tan);
        pos = current_bunker_pos + (PIXEL_SIZE_GLOBAL * 24) * 6;
    }
}

void bunker_detect_hits() {
    bunker_detect_hit_player();
    for (uint8_t i = 0; i < MAX_BULLETS; i++) {
        if (alien_bullet_moving[i]) {
            bunker_detect_hit_alien(i);
        }
    }
}

void bunker_detect_hit_player()
{
    if (!bullet_moving) {
        return;
    }

    for (int16_t i = 0; i < NUM_BUNKERS; i++) // loop through the 5 Y aliens
    {
        for (int16_t j = 0; j < NUM_BLOCKS; j++) // loop through the 11 X aliens
        {  
            // Set x borders
            uint32_t left_border = bunker_pos[i][j];
            uint32_t right_border = bunker_pos[i][j] + 6 * PIXEL_SIZE_GLOBAL * SIZE_SCALAR;

            // Set y borders
            uint32_t top_border = bunker_pos[i][j];
            uint32_t bot_border = bunker_pos[i][j] + 6 * NEW_LINE * SIZE_SCALAR;

            // Get coord values for the Y borders
            uint16_t top_border_y = draw_alien_get_y_coord(top_border);
            uint16_t bot_border_y = draw_alien_get_y_coord(bot_border);

            // Get the coord values based of all of those
            uint16_t bullet_y = draw_alien_get_y_coord(current_pos_bullet);
            uint16_t bullet_x = draw_alien_get_x_coord(current_pos_bullet, bullet_y);
            uint16_t left_border_y = draw_alien_get_y_coord(left_border);
            uint16_t right_border_y = draw_alien_get_y_coord(right_border);
            uint16_t left_border_x = draw_alien_get_x_coord(left_border, left_border_y);
            uint16_t right_border_x = draw_alien_get_x_coord(right_border, right_border_y);       

            // Check if a bullet is within one of the bunker blocks
            if ((bunker_damage[i][j] != bunkerGone_6x6) &&
                (bullet_x >= left_border_x) &&
                (bullet_x <  right_border_x) &&
                (bullet_y >= top_border_y) &&
                (bullet_y <  bot_border_y))
            {

                // printf("Bullet: (%d, %d)\n\r", bullet_x, bullet_y);
                // printf("%d <= X < %d\n\r", left_border_x, right_border_x);
                // printf("%d <= Y < %d\n\n\r", top_border_y, bot_border_y);


                // Change sprite to the explosion and set boolean flag
                bunker_damage[i][j] = bunker_take_damage(bunker_damage[i][j]);

                // Grab new sprite val;
                uint32_t new_sprite[6];
                for (int k = 0; k < 6; k++) {
                    new_sprite[k] = bunker_damage[i][j][k] & bunker_sprites[i][j][k];
                }

                // Draw new damage
                draw_alien(new_sprite, bunker_pos[i][j], 6, 6, PIXEL_SIZE_GLOBAL * SIZE_SCALAR, tan);

                // Erase bullet
                bullet_moving = false; // make sure bullet stops travelling
                draw_alien(tankbullet_1x5, current_pos_bullet, 1, 5, PIXEL_SIZE_GLOBAL * SIZE_SCALAR, black); // erase bullet
            }
        }
    }
}

void bunker_redraw_all() {
    for (int16_t i = 0; i < NUM_BUNKERS; i++) // loop through the 5 Y aliens
    {
        for (int16_t j = 0; j < NUM_BLOCKS; j++) // loop through the 11 X aliens
        {  
            uint32_t new_sprite[6];
            for (int k = 0; k < 6; k++) {
                new_sprite[k] = bunker_damage[i][j][k] & bunker_sprites[i][j][k];
            }

            // Draw new damage
            draw_alien(new_sprite, bunker_pos[i][j], 6, 6, PIXEL_SIZE_GLOBAL * SIZE_SCALAR, tan);
        }
    }
}

void bunker_detect_hit_alien(uint8_t bullet_num)
{
    for (int16_t i = 0; i < NUM_BUNKERS; i++) // loop through the 5 Y aliens
    {
        for (int16_t j = 0; j < NUM_BLOCKS; j++) // loop through the 11 X aliens
        {  
            uint32_t bullet_bottom_left = alien_bullet_pos[bullet_num] + SIZE_SCALAR * NEW_LINE * 5;
            uint32_t bullet_bottom_right = bullet_bottom_left + SIZE_SCALAR * PIXEL_SIZE_GLOBAL * 3;

            // Set x borders
            uint32_t left_border = bunker_pos[i][j];
            uint32_t right_border = bunker_pos[i][j] + 6 * PIXEL_SIZE_GLOBAL * SIZE_SCALAR;

            // Set y borders
            uint32_t top_border = bunker_pos[i][j];
            uint32_t bot_border = bunker_pos[i][j] + 6 * NEW_LINE * SIZE_SCALAR;

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
            if ((bunker_damage[i][j] != bunkerGone_6x6) &&
                (bullet_bottom_left_x >= left_border_x  || bullet_bottom_right_x >= left_border_x)  &&
                (bullet_bottom_left_x <  right_border_x || bullet_bottom_right_x <  right_border_x) &&
                (bullet_bottom_y >= top_border_y) &&
                (bullet_bottom_y <  bot_border_y))
            {

                // printf("Bullet: (%d, %d)\n\r", bullet_x, bullet_y);
                // printf("%d <= X < %d\n\r", left_border_x, right_border_x);
                // printf("%d <= Y < %d\n\n\r", top_border_y, bot_border_y);

                // Change sprite to the explosion and set boolean flag
                bunker_damage[i][j] = bunker_take_damage(bunker_damage[i][j]);

                // Grab new sprite val;
                uint32_t new_sprite[6];
                for (int k = 0; k < 6; k++) {
                    new_sprite[k] = bunker_damage[i][j][k] & bunker_sprites[i][j][k];
                }

                // Draw new damage
                draw_alien(new_sprite, bunker_pos[i][j], 6, 6, PIXEL_SIZE_GLOBAL * SIZE_SCALAR, tan);

                // Erase bullet
                alien_bullet_moving[bullet_num] = false; // make sure bullet stops travelling
                draw_alien(alienbullet2_down_3x5, alien_bullet_pos[bullet_num], 3, 5, PIXEL_SIZE_GLOBAL * SIZE_SCALAR, black); // erase bullet
            }
        }
    }
}