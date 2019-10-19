#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "../globals/globals.h"
#include "../scores/scores.h"

void game_over_print_score(score_t score, uint32_t pos);

void game_over_print_scores();

void game_over_blink_cursor(bool force);

#endif