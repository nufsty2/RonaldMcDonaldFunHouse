#ifndef DRAW_UI_H
#define DRAW_UI_H

#include "../globals/globals.h"

void draw_ui_init();

void update_score(bool firstRun);

void draw_ui_update_lives(bool firstRun);

char* get_score_digit(uint8_t digit);

char* get_selected_char();

void draw_ui_increase_score(uint16_t row);

void draw_ui_increase_score_saucer();

void draw_ui_increment_lives();

void draw_ui_decrement_lives();

#endif