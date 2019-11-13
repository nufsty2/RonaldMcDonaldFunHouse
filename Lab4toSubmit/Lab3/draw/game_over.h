#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "../globals/globals.h"
#include "../scores/scores.h"

// This is our main function to print main scores
// @param score - the score to pass in
// @param pos - the position of the number
void game_over_print_score(score_t score, uint32_t pos);

// This is our main function print all the scores
void game_over_print_scores();

// This is our blink cursor function to blink the selected cursor
// @param force - forces the color change of the letter
void game_over_blink_cursor(bool force);

#endif