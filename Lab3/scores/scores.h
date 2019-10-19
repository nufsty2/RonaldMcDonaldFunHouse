#ifndef SCORES_H
#define SCORES_H

#include <stdio.h>
#include <stdint.h>
#include "../globals/globals.h"

#define NAME_LENGTH 3
#define SCORE_LENGTH 5

// Struct to represent the score
// value - int value of the score
// name - 3 char name associated with the score
// value_char - the score converted into char to display to the screen
typedef struct score_t 
{
    uint32_t value;
    char name[NAME_LENGTH];
    char value_char[SCORE_LENGTH];
} score_t;

// This function gets only the top ten scores to display on the screen
// @param top_ten - array of the top ten scores after the sort
void scores_get_top_10_scores();

// This function writes the scores to the text file
// @param new_score - the new score to append to the text file
void scores_write_new_score(score_t new_score);

#endif