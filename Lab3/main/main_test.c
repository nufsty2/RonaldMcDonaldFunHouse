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
    char black[3] = {0xFF, 0xFF, 0xFF};
    for (int i = 0; i < 640; i++)
    {
        if (i%3==0)
        {
            seek_hdmi(i);
            write_hdmi(black);
            printf("%d\n\r", read_hdmi(3));
        }
    }

    close_hdmi();
}