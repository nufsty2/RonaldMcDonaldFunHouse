#ifndef PLAYER_H
#define PLAYER_H

#include "../globals/globals.h"

// This function fires the bullet for the player
void player_fire_bullet();

// This function actually moves the player
void player_move();

// This function detects alien bullet hits on the player
void player_detect_alien_hit();

#endif