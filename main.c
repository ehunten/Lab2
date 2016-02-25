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

//DEFINES
#define OFF 0
#define LATG13 LATGbits.LATG13
#define LATG0 LATGbits.LATG0
#define LATF1 LATFbits.LATF1
#define LATG12 LATGbits.LATG12

#define PORTD5 PORTDbits.RD5
#define PORTD11 PORTDbits.RD11
#define PORTC14 PORTCbits.RC14

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
    
    
    int keyNum = 0;
    
    while (1) {
        
       
        initLCD();

        switch (state) {
            case init: printStringLCD("Yooooo");
                state = wait;
                break;
            
            case wait:
                break;
                
            case keyPress: printStringLCD("Key press!");
                break;
                
            case writeKey: printCharLCD(keyNum);
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
        state = keyPress;
    }
    
    else if (IFS1bits.CNCIF == 1) {
        IFS1bits.CNCIF = OFF;
        state = keyPress;
    }
    
}