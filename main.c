// ******************************************************************************************* //
//
// File:         main.c
// Date:         2/25/16
// Authors:      Evelyn Hunten
//
// Description: Part 1 for lab 2
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "keypad.h"
#include "config.h"
#include "timer.h"

//DEFINES

int main(void)
{
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();                  
    initTimer1();
    initTimer2();
    
    initLCD();

    while (1) {
        testLCD();
    }

    
    return 0;
}


