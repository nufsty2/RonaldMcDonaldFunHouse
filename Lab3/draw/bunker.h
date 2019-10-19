#ifndef BUNKER_H
#define BUNKER_H

#include "../globals/globals.h"

// This is our init function, it sets up the four starting bunkers on the game load
void bunker_init();

// This is our function to detect alien or player hits on the bunker
void bunker_detect_hits();

// This function is redraw the bunker if it has taken damage
void bunker_redraw_all();

void bunker_detect_hit_player();

void bunker_detect_hit_alien(uint8_t bullet_num);

#endif