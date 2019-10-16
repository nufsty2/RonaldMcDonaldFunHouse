#ifndef DRAW_GAME_OVER_H
#define DRAW_GAME_OVER_H

#include "../globals/globals.h"
#include "../sprites/sprites.c"
#include "../scores/scores.h"

void main_print_score(score_t score, uint32_t pos);

void main_print_scores();

void blink_cursor(bool force);

#endif