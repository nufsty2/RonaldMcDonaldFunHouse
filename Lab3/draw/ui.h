#ifndef UI_H
#define UI_H

#include "../globals/globals.h"

void ui_init();

void ui_update_score(bool firstRun);

void ui_update_lives(bool firstRun);

char* ui_get_score_digit(uint8_t digit);

char* ui_get_selected_char();

void ui_increase_score(uint16_t row);

void ui_increase_score_saucer();

void ui_increment_lives();

void ui_decrement_lives();

#endif