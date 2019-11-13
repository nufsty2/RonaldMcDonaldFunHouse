#ifndef UI_H
#define UI_H

#include "../globals/globals.h"

// Inits what we need in the UI interface
void ui_init();

// This function updates the score
// @param firstRun - If first run, make it zero
void ui_update_score(bool firstRun);

void ui_update_lives(bool firstRun);

// This function gets the score digit
// @param - digit - the digit we want to get
// @return - returns address of the score digit
char* ui_get_score_digit(uint8_t digit);

// This function gets the char
// @ return - address of the score char
char* ui_get_selected_char();

// This function increments the score
// @param row - whatever row it's on
void ui_increase_score(uint16_t row);

// This incs the score randomly for the saucer
void ui_increase_score_saucer();

// This function incs the lives
void ui_increment_lives();

// This function decremnts the lives
void ui_decrement_lives();

#endif