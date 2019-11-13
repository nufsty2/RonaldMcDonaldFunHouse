#include "player.h"
#include "ui.h"
#include "../sprites/sprites.c"
#include "alien.h"
#include "../../Lab4/audio/user_audio.h"

/* Defines for this c file */
#define PLAYER_BULLET_SPEED 8
#define RIGHT_MOST_WALL 1839
#define PLAYER_SPEED 4
#define MOVE_OFFSET 13
#define MOVE_OFFSET_SCALAR 6

/* Player attributes */
extern uint32_t current_pos_bullet;
extern uint32_t current_pos_player;
extern bool bullet_moving;
extern bool player_dead;
extern bool new_shot;

/* Colors */
extern char black[];
extern char cyan[];

/* Button value */
extern uint32_t buttons_val;

/* Alien attributes */
extern uint32_t alien_bullet_pos[];
extern bool alien_bullet_moving[];

/* */
extern uint32_t sound_player_die_ctr;

// This function fires a bullet
void player_fire_bullet()
{
    if (new_shot) 
    {
        user_audio_play_sound(SOUND_LASER);
        new_shot = false;
    }

    // Declare variables
    uint32_t old_pos_bullet = current_pos_bullet;

    uint16_t y_coord = alien_get_y_coord(current_pos_bullet);
    
    // Move bullet up
    old_pos_bullet = current_pos_bullet;
    current_pos_bullet -= (GLOBALS_NEW_LINE*PLAYER_BULLET_SPEED); // make bullet go up

    alien_draw(tankbullet_gone_1x5, old_pos_bullet, GLOBALS_PLAYER_BULLET_WIDTH, GLOBALS_PLAYER_BULLET_HEIGHT, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black); // erase old bullet

    alien_draw(tankbullet_1x5, current_pos_bullet, GLOBALS_PLAYER_BULLET_WIDTH, GLOBALS_PLAYER_BULLET_HEIGHT, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, cyan); // draw new bullet

    // At this point, bullet gone
    if (current_pos_bullet < (GLOBALS_NEW_LINE * GLOBALS_TANK_WIDTH)) 
    {
        bullet_moving = false;
        alien_draw(tankbullet_1x5, current_pos_bullet, GLOBALS_PLAYER_BULLET_WIDTH, GLOBALS_PLAYER_BULLET_HEIGHT, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black); // erase old bullet
        new_shot = true;
    }
}

// This function moves the player right and make sure he doesn't loop over the screen
void player_move_right()
{
    if ((current_pos_player+GLOBALS_TANK_WIDTH) % GLOBALS_NEW_LINE != RIGHT_MOST_WALL)
    {
        alien_draw(block_2x8, current_pos_player, GLOBALS_BLOCK_WIDTH, GLOBALS_BLOCK_HEIGHT, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black);

        current_pos_player += GLOBALS_PIXEL_SIZE * PLAYER_SPEED;

        alien_draw(tank_15x8, current_pos_player, GLOBALS_TANK_WIDTH, GLOBALS_TANK_HEIGHT, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, cyan);
    }
}

// This function moves the player left and make sure he doesn't loop over the screen
void player_move_left()
{
    if (current_pos_player % GLOBALS_NEW_LINE != 0)
    {
        alien_draw(block_2x8, current_pos_player+MOVE_OFFSET*MOVE_OFFSET_SCALAR, GLOBALS_BLOCK_WIDTH, GLOBALS_BLOCK_HEIGHT, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, black);

        current_pos_player -= GLOBALS_PIXEL_SIZE * PLAYER_SPEED;

        alien_draw(tank_15x8, current_pos_player, GLOBALS_TANK_WIDTH, GLOBALS_TANK_HEIGHT, GLOBALS_PIXEL_SIZE*GLOBALS_SIZE_SCALAR, cyan);
    }
}

// This function combines the two above
void player_move()
{
    if (buttons_val == GLOBALS_BTN_0)
        player_move_right();
    else if (buttons_val == GLOBALS_BTN_1)
        player_move_left();
}

// This function destroys the player on alien bullet hit
void player_destroy()
{
    player_dead = true;
    ui_decrement_lives(); // magic
}

// This function detects alien bullet hits on the player
void player_detect_alien_hit() 
{
    for (uint8_t i = 0; i < GLOBALS_MAX_BULLETS; i++)
    {
        uint32_t bullet_bottom_left = alien_bullet_pos[i] + GLOBALS_SIZE_SCALAR * GLOBALS_NEW_LINE * GLOBALS_ALIEN_BULLET_HEIGHT;
        uint32_t bullet_bottom_right = bullet_bottom_left + GLOBALS_SIZE_SCALAR * GLOBALS_PIXEL_SIZE * GLOBALS_ALIEN_BULLET_WIDTH;

        // Set x borders
        uint32_t left_border = current_pos_player;
        uint32_t right_border = current_pos_player + GLOBALS_TANK_WIDTH * GLOBALS_PIXEL_SIZE * GLOBALS_SIZE_SCALAR;

        // Set y borders
        uint32_t top_border = current_pos_player;
        uint32_t bot_border = current_pos_player + GLOBALS_TANK_HEIGHT * GLOBALS_NEW_LINE * GLOBALS_SIZE_SCALAR;

        // Get coord values for the Y borders
        uint16_t top_border_y = alien_get_y_coord(top_border);
        uint16_t bot_border_y = alien_get_y_coord(bot_border);

        // Get the coord values based of all of those
        uint16_t bullet_bottom_y = alien_get_y_coord(bullet_bottom_left);
        uint16_t bullet_bottom_left_x = alien_get_x_coord(bullet_bottom_left, bullet_bottom_y);
        uint16_t bullet_bottom_right_x = alien_get_x_coord(bullet_bottom_right, bullet_bottom_y);
        uint16_t left_border_y = alien_get_y_coord(left_border);
        uint16_t right_border_y = alien_get_y_coord(right_border);
        uint16_t left_border_x = alien_get_x_coord(left_border, left_border_y);
        uint16_t right_border_x = alien_get_x_coord(right_border, right_border_y);       

        // Check if a bullet is within the player block
        if ((bullet_bottom_left_x >= left_border_x  || bullet_bottom_right_x >= left_border_x)  &&
            (bullet_bottom_left_x <  right_border_x || bullet_bottom_right_x <  right_border_x) &&
            (bullet_bottom_y >= top_border_y) &&
            (bullet_bottom_y <  bot_border_y))
        {
            // Erase bullet
            alien_bullet_moving[i] = false; // make sure bullet stops travelling
            alien_draw(alienbullet2_down_3x5, alien_bullet_pos[i], GLOBALS_ALIEN_BULLET_WIDTH, GLOBALS_ALIEN_BULLET_HEIGHT, GLOBALS_PIXEL_SIZE * GLOBALS_SIZE_SCALAR, black); // erase bullet
            player_destroy();
            user_audio_play_sound(SOUND_PLAYER_DIE);
            // Start incrementing the sound counter
            sound_player_die_ctr++;

            return;
        }
    }
}