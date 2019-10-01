#include <stdint.h>
#include <stdio.h>
#include "../hdmi/hdmi.h"

int main()
{
    /* HDMI Open Test */
    int16_t open_hdmi_code = open_hdmi();
    if (open_hdmi_code == 1)
    {
        printf("Open hdmi success\n\r");
    }
    else
    {
        printf("Open hdmi failed: %d\n\r", open_hdmi_code);
    }

    /* HDMI Write/Read Test */
    char white[3] = {0xFF, 0xFF, 0xFF}; // single pixel
    for (int i = 0; i < 640*5; i++) // write white to first 5 rows
    {
        seek(i);
        write_hdmi(white); // Write the color to the screem
    }

    close_hdmi();
}