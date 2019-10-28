#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct wavfile
{
    char   id[4];            // should always contain "RIFF"
    int    totallength;      // total file length minus 8
    char   wavefmt[8];       // should be "WAVEfmt "
    int    format;           // 16 for PCM format
    short  pcm;              // 1 for PCM format
    short  channels;         // channels
    int    frequency;        // sampling frequency
    int    bytes_per_second;
    short  bytes_by_capture;
    short  bits_per_sample;
    char   data[4];          // should always contain "data"
    int    bytes_in_data;
};

// COPIED FROM: http://yannesposito.com/Scratch/en/blog/2010-10-14-Fun-with-wav/
void readWAVFile(char waveFileName[])
{
    FILE* fp = fopen(waveFileName, "rb");
    wavefile info;
    if (fp)
    {
        fread(&info, sizeof(wavefile), fp);
    }
}