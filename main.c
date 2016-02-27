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
#include "lcd.h"
#include "interrupt.h"

//DEFINES
#define ENABLE 1
#define OFF 0
#define PORTC1 PORTDbits.RD5
#define PORTC2 PORTDbits.RD11
#define PORTC3 PORTCbits.RC14


typedef enum stateTypeEnum{
    init, wait, keyPress, writeKey
} stateType;

volatile stateType state = init;


int main(void)
{
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTimer1();
    initTimer2();
    initLCD();
    clearLCD();

    char key;

    while (1) {


        switch (state) {
            case init: printStringLCD("Yooooo");
                state = wait;
                break;
             
                //this case probably isn't necessary but whatever
            case wait: openScanning();
                break;
                
            case keyPress: key = scanKeypad();
                 state = writeKey;
                 break;

            case writeKey: printCharLCD(key);
                 state = wait;
                 break;

            default: printStringLCD("Ruh roh!");
                    break;
        }
    }


    return 0;
}


//Button Interrupt
void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void){

    PORTD;
    PORTC;

    if (IFS1bits.CNDIF == 1){
        IFS1bits.CNDIF = OFF;
        if (PORTC1 == 0 | PORTC2 == 0) {
            state = keyPress;
        }
    }


    else if (IFS1bits.CNCIF == 1) {
        IFS1bits.CNCIF = OFF;
        if (PORTC3 == 0) {
            state = keyPress;
        }
    }

}
