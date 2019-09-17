// Button/Switch test code file
// Modify the CMake file to run this file

// FOR THIS TO WORK: YOU NEED TO chmod 777 ALL UIO UNDER /dev/uio0, uio1, uio2, uio3, uio4

#include <stdio.h>

#include "../buttons/buttons.h"
#include "../switches/switches.h"
#include "../intc/intc.h"

int main()
{
    printf("Button Test Started\n\r");

    /******************* Init Button Test *************************/
    int32_t initSuccess = init_buttons("/dev/uio1");

    if (initSuccess == 1)
    {
        printf("Button Init Success\n"); // good
    }
    else
    {
        printf("Button Init Failed\nError code: %d\n\r", initSuccess); // bad
    }
    /**************************************************************/

    /******************* Read Button Test *************************/
    printf("Button Test Started (no debounce). Press Button 0 to quit\n\r");
    while (1)
    {
        uint32_t buttonRead = read_buttons(0);

        if (buttonRead == 1)
        {
            break; // exit loop
        }
        if (buttonRead == 2)
        {
            printf("Button 1 Pressed\n");
        }
        if (buttonRead == 4)
        {
            printf("Button 2 Pressed\n");
        }
        if (buttonRead == 8)
        {
            printf("Button 3 Pressed\n");
        }
    }
    /**************************************************************/
 
    printf("Button Test Done\n\rSwitch Test Started\n\r");

    /******************* Init Switches Test *************************/
    int32_t initSSuccess = init_switches("/dev/uio2");

    if (initSSuccess == 1)
    {
        printf("Switch Init Success\n"); // good
    }
    else
    {
        printf("Switch Init Failed\nError code: %d\n\r", initSSuccess); // bad
    }
    /****************************************************************/

    /******************* Read Switches Test *************************/
    printf("Switch Read Test Started\n\rFlip both switches up to exit loop");

    while(1)
    {
        uint32_t switchValue = read_switches(0);

        if (switchValue == 0) // both switches are low
        {
            printf("Both Switches Low\n\r");
        }
        else if (switchValue == 1)
        {
            printf("SW0 is high, SW1 is low\n\r");
        }
        else if (switchValue == 2)
        {
            printf("SW0 is low, SW1 is high\n\r");
        }
        else if (switchValue == 3)
        {
            break;
        }
    }

    printf("Switches Test Done\n\r");
    /****************************************************************/

    /******************* Init INTC Test *************************/
    int32_t initISuccess = intc_init("/dev/uio4");

    if (initISuccess == 1)
    {
        printf("INTC Init Success\n"); // good
    }
    else
    {
        printf("INTC Init Failed\nError code: %d\n\r", initISuccess); // bad
    }
    /************************************************************/


    printf("All Tests Done\n\r");
    
    // EXIT
    buttons_exit();
    switches_exit();
    intc_exit();

    return 0;
}