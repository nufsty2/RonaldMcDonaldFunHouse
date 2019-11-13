#include "wav.h"

#define WORD_SIZE 4
#define DATA_START (44L)
#define BYTES_PER_READ 1

// This function reads the wav file
// @param array - this reads in an unitalized array
// @param input - this is the file path of the .wav (../something.wav)
// @return - returns the number of samples and array now has all the values
uint32_t wav_read_data(int32_t *array, char * input)
{
    uint32_t num_samples = 0;
    FILE * fp;

    if ((fp = fopen(input, "rb")) == NULL) // cant open
    {
        printf("File pointer in wav.c is null\n\r");
        return 0; // return nothing
    }

    fseek(fp, DATA_START, 0); // read from 44 byte to end (data)

    // Setting init values
    int16_t value = 0;
    int32_t value32 = 0;
    uint32_t i = 0;
    while (fread(&value, sizeof(value), BYTES_PER_READ, fp)) // actually reads the file
    {
        value32 = value << WORD_SIZE; // shift by word size because it's 16 bit samples
        array[i++] = value32; // copy into array
        num_samples++; // inc the num samples
    }
    return num_samples * WORD_SIZE;
}