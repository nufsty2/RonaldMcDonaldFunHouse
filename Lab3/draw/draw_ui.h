#ifndef DRAW_UI_H
#define DRAW_UI_H

#include <stdbool.h>
#include "../globals/globals.h"
#include "../sprites/sprites.c"

void update_score(bool firstRun);

void update_lives(bool firstRun);

char* get_score_digit(uint8_t digit);

char* get_selected_char();

#endif