#include "scores.h"

#define MAX_CHAR 1000
#define TONS_OF_HIGH_SCORES 100000

uint32_t num_scores = 0;

score_t* scores_insertion_sort(score_t scores[]) {
    uint32_t j;

    for (uint32_t i = 1; i < num_scores; i++) 
    {
        j = i;

        while (j > 0 && scores[j].value > scores[j-1].value) 
        {
            score_t temp = scores[j];
            scores[j] = scores[j-1];
            scores[j - 1] = temp;
            j--;
        }
    }
    return scores;
}

score_t* scores_get_top_10_scores() 
{
    score_t high_scores[TONS_OF_HIGH_SCORES];
    FILE *ptr_file;
    char buf[MAX_CHAR];

    ptr_file = fopen("../scores/high_scores.txt", "r");
    if (!ptr_file)
        return NULL;

    while (fgets(buf, MAX_CHAR, ptr_file) != NULL)
    {
        char score_char[5] = 
        {
            buf[4],
            buf[5],
            buf[6],
            buf[7],
            buf[8]
        };
        uint32_t score_int;
        sscanf(score_char, "%d", &score_int);
        score_t newScore = {.name = {buf[0], buf[1], buf[2]}, 
                            .value = score_int,
                            .value_char = score_char};
        high_scores[num_scores] = newScore;
        num_scores++;
    }

    fclose(ptr_file);
    scores_insertion_sort(high_scores);

    score_t top_ten[10];
    for (uint8_t i = 0; i < 10; i++) {
        top_ten[i] = high_scores[i];
    }
    return top_ten;
}
