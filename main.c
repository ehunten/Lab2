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
    init, open, wait, dbC1, dbD1, keyPress, nextKey
} stateType;

volatile stateType state = init;


int main(void)
{
    
    SYSTEMConfigPerformance(10000000);
    initKeypad();
    enableInterrupts();
    initTimer1();
    initTimer2();
    initLCD();

    clearLCD();

    
    char key;
    int cursorPos = 1;

    while (1) {


        switch (state) {
            case init:
                clearLCD();
                state = open;
                break;
             
            case open:
                openScanning();
                state = wait;
                break;
                
            case wait: 
                break;
                
            case dbC1: delayUs(50);
            state = keyPress;
                break;
                
            case dbD1: delayUs(50);
            state = keyPress;
                break;
                
                
            case keyPress:
                printCharLCD(scanKeypad());
                cursorPos++;
                if (cursorPos == 17) {
                    moveCursorLCD(0); 
                }
                else if (cursorPos == 33) {
                    moveCursorLCD(1);
                    cursorPos = 0;
                }
                 state = nextKey;
                 break;

            case nextKey:
                 //moveCursorLCD();
                 state = open;
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
        if (PORTC1 == 0) {
            if (state == wait) {state = dbD1; }
        }
        
        else if  (PORTC2 == 0) {
            if (state == wait) {state = dbD1;}
        }
        
        else if (PORTC1 == 1) {
            // (state == dbD1) {state = dbD2; }
            
        }
        
        else if (PORTC2 == 1) {
            //if (state == dbD1) {state = dbD2; }
        } 
    }


    else if (IFS1bits.CNCIF == 1) {
        IFS1bits.CNCIF = OFF;
        if (PORTC3 == 0) {
            if (state == wait) { state = dbC1; }
        }
        else if (PORTC3 == 1) {
            //if (state == dbC1) { state = dbC2; }
        }
    }
    
   

}
