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
#include "password.h"

//DEFINES
#define ENABLE 1
#define OFF 0
#define PORTC1 PORTDbits.RD5
#define PORTC2 PORTDbits.RD11
#define PORTC3 PORTCbits.RC14


typedef enum stateTypeEnum{
    enter, open, wait, debounce, setMode, entryWait, entryMode, setWait, entryOpen, setOpen
} stateType;

volatile stateType state = enter;
volatile stateType nextState = enter;

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
    int star = 0;

    while (1) {

             
            switch (state) {
                case enter:
                    clearLCD();
                    printStringLCD("Enter:")
                    moveCursorLCD(0);
                    state = wait;
                    break;

                case wait:
                    break;

                case debounce:
                    delayUs(50);
                    state = nextState;
                    break;

                case entryMode:
                    key = scanKeypad();
                    printCharLCD(key);
                    state = entryOpen;

                    if (key == '*') {
                        star++;
                        if (star == 2) {
                            state = setOpen;
                            moveCursorLCD(1);
                            clearLCD();
                            printStringLCD("Set Mode")
                            star = 0;
                        }
                        else if (star == 1 & (key != '*' )) {
                            star = 0;
                            moveCursorLCD(1);
                            printStringLCD("Bad Entry");
                        }
                    }
                    else if (key == '#') {
                        clearLCD();
                        moveCursorLCD(1);
                        printStringLCD("Bad Entry");
                    }
  
                    //CODE FOR STORING/CHECKING PASSWORDS

                    break;

                case entryOpen:
                    openScanning();
                    state = entryWait;
                    break;
                
                case entryWait:
                    break;
                
                case setOpen:
                    openScanning();
                    state = setWait;
                 
                case setMode:
                    moveCursorLCD(0);
                    key = scanKeypad();
                    printCharLCD(key);
                    state = setOpen;
                    
                    if (key == '*') {
                        star++;
                        if (star == 2) {
                            state = setOpen;
                            moveCursorLCD(1);
                            clearLCD();
                            printStringLCD("Set Mode")
                            star = 0;
                        }
                        else if (star == 1 & (key != '*' )) {
                            star = 0;
                            moveCursorLCD(1);
                            printStringLCD("Bad Entry");
                        }
                    }
                    else if (key == '#') {
                        clearLCD();
                        moveCursorLCD(1);
                        printStringLCD("Bad Entry");
                    }
                    //FUNCTIONS FOR VALIDATING AND SAVING PASSWORDS
                    break;
                    
                case setWait:
                    
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
            if (state == wait | state == entryWait) {
              nextState = entryMode;
              state = debounce; 
            }
            else if (state == setMode | state == setWait) {
              nextState = setMode;
              state = debounce;
            }
        }

      }
                     
        else if (PORTC1 == 1) {
        }    
        else if (PORTC2 == 1) {
        } 
  

    else if (IFS1bits.CNCIF == 1) {
        IFS1bits.CNCIF = OFF;
        if (PORTC3 == 0) {
          if (state == wait | state == entryWait) {
            nextState = entryMode;
            state = debounce; 
          }
          else if (state == setMode | state == setWait) {
            nextState = setMode;
            state = debounce;
          }
        }
        else if (PORTC3 == 1) {
        }
    }
  }
    
