#include "bunker.h"
#include "../sprites/sprites.c"

#define NUM_BUNKERS 4
#define NUM_BLOCKS  10

/* Externs */
extern uint32_t current_pos_bullet;
extern char tan[];

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

void bunkers_init()
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

void bunker_detect_hit()
{
    // Create the borders for the bunker

    // Check to see if the bullet is in the borders

    // Check to make sure that specific bunker part is not dead, if it is, break

    // Update bunker state, set bullet to false
}