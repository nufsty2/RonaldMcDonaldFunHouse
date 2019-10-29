#include "wav.h"


int32_t* wav_read_data(char * input)
{

    FILE * fp;

    if ((fp = fopen(input, "rb")) == NULL) // cant open
    {
        printf("FP is null mmkay\n\r");
        return NULL;
    }

    int32_t data_to_return[50000];
    //fseek(fp, 44, 0); // read from 44 byte to end (data)

    int16_t value = 0;
    int32_t value32 = 0;
    uint32_t i = 0;
    while (fread(&value,sizeof(value),1,fp))
    {
        if (value < 0) value = -value;
        value32 = value << 8;
        printf("%x\n\r", value32);
        data_to_return[i++] = value32;
    }

    return &(data_to_return[0]);
}