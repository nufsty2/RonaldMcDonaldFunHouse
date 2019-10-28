#include "wav.h"

typedef struct header 
{
    char chunk_id[4];
    int chunk_size;
    char format[4];
    char subchunk1_id[4];
    int subchunk1_size;
    short int audio_format;
    short int num_channels;
    int sample_rate;
    int byte_rate;
    short int block_align;
    short int bits_per_sample;
    short int extra_param_size;
    char subchunk2_id[4];
    int subchunk2_size;
} header;

typedef struct header* header_p;

int32_t* wav_read_data(char * input)
{
    FILE * infile = fopen(input, "rb");

    int BUFSIZE = 512;

    // used for processing 16-bit file
    int16_t inbuff16[BUFSIZE];
    int32_t inbuff32[BUFSIZE];


    // header_p points to a header struct that contains the file's metadata fields
    header_p meta = (header_p)malloc(sizeof(header));

    if (infile)
    {
        // read and write header data
        fread(meta, 1, sizeof(header), infile);

        while (!feof(infile))
        {
            fread(inbuff16, 1, BUFSIZE*2, infile); // *2 = read 16 bits at a time rather than 8
        }

        for (int i = 0; i < BUFSIZE; i++)
        {
            inbuff32[i] = inbuff16[i] << 8; // left shift by 8
            if (inbuff32[i] < 0) inbuff32[i] = -inbuff32[i];
            printf("Value = %x\n\r", inbuff32[i]);
        }
    }

    return inbuff32;
}