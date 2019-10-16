#ifndef SCORES_H
#define SCORES_H

#include <stdio.h>
#include <stdint.h>
#include "../globals/globals.h"

typedef struct score_t {
    uint32_t value;
    char name[4];
    char value_char[5];
} score_t;

void scores_get_top_10_scores();

void scores_write_new_score(score_t new_score);

#endif