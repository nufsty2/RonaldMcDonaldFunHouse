#include "sprites.c"
#include <stdbool.h>

typedef struct invader_t
{
    bool isAlive;
    bool isMoving;
    const uint32_t sprite[];
} invader_t;

invader_t alien_top = {
    .isAlive = true,
    .isMoving = true,
    .sprite = alien_top_in_12x8
};

invader_t alien_middle = {
    .isAlive = true,
    .isMoving = true,
    .sprite = alien_middle_in_12x8
};

invader_t alien_bottom = {
    .isAlive = true,
    .isMoving = true,
    .sprite = alien_bottom_in_12x8
};

invader_t alien_army[5][11] = {
    {alien_top,    alien_top,    alien_top,    alien_top,    alien_top,    alien_top,    alien_top,    alien_top,    alien_top,    alien_top,    alien_top   },
    {alien_middle, alien_middle, alien_middle, alien_middle, alien_middle, alien_middle, alien_middle, alien_middle, alien_middle, alien_middle, alien_middle},
    {alien_middle, alien_middle, alien_middle, alien_middle, alien_middle, alien_middle, alien_middle, alien_middle, alien_middle, alien_middle, alien_middle},
    {alien_bottom, alien_bottom, alien_bottom, alien_bottom, alien_bottom, alien_bottom, alien_bottom, alien_bottom, alien_bottom, alien_bottom, alien_bottom},
    {alien_bottom, alien_bottom, alien_bottom, alien_bottom, alien_bottom, alien_bottom, alien_bottom, alien_bottom, alien_bottom, alien_bottom, alien_bottom}
};