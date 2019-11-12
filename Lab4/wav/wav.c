#include "wav.h"


uint32_t wav_read_data(int32_t *array, char * input)
{
    uint32_t num_samples = 0;
    FILE * fp;

    if ((fp = fopen(input, "rb")) == NULL) // cant open
    {
        printf("FP in wav.c is null mmkay\n\r");
        return 0;
    }

    fseek(fp, 44L, 0); // read from 44 byte to end (data)

    int16_t value = 0;
    int32_t value32 = 0;
    uint32_t i = 0;
    while (fread(&value,sizeof(value),1,fp))
    {
        if (value < 0) value = -value;
        value32 = value << 8;
        //printf("%x\n\r", value32);
        array[i++] = value32;
        num_samples++;
    }
    return num_samples * 4;
}