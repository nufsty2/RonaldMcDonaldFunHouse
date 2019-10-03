#include <stdio.h>
#include <stdint.h>

typedef struct score_t {
    uint32_t value;
    char value_char[5];
    char name[4];
} score_t;

score_t* scores_get_top_10_scores();
