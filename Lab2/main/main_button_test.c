// Button test code file
#include <stdio.h>

#include "../buttons/buttons.h"

int main()
{
    int16_t testSuccess = init_buttons("/dev/uio1");

    if (testSuccess == -1)
    {
        printf("Init Failed\n");
    }
    else if (testSuccess == 1)
    {
        printf("Init Success\n");
    }
    else
    {
        printf(testSuccess);
    }

    printf("done\n");
}