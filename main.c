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
//#include "password.h"

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
volatile stateType nextState = entryMode;

volatile char passwords[4][4];
volatile char temp[4];
volatile int currRow = 0;

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
    int entry = 0;
    int i = 0;
    
    initPasswords();

    while (1) {

             
            switch (state) {
                case enter:
                    clearLCD();
                    printStringLCD("Enter: ");
                    moveCursorLCD(0);
                    state = wait;
                    break;

                case wait: printStringLCD("WAIT");
                clearLCD();
                    break;

                case debounce: printStringLCD("debounce");
                clearLCD();
                    delayUs(50);
                    state = nextState;
                    //state = entryMode;
                    break;

                case entryMode:
                    clearLCD();
                    printStringLCD("Entry Mode");
                    key = scanKeypad();
                    moveCursorLCD(0);
                    printCharLCD(key);
                    state = entryWait;

                    if (key == '*') {
                        entry = 0;
                        star++;
                        if (star == 2) {
                            state = setWait;
                            moveCursorLCD(1);
                            clearLCD();
                            printStringLCD("Set Mode");
                            star = 0;
                            //break;
                        }
                        else if (star == 1 & (key != '*' )) {
                            star = 0;
                            moveCursorLCD(1);
                            printStringLCD("Bad Entry");
                            //break;
                        }
                    }
                    else if (key == '#') {
                        entry = 0;
                        clearLCD();
                        moveCursorLCD(1);
                        printStringLCD("Bad Entry");
                        //break;
                    }
  
                    else {
                        star = 0;
                        if (entry < 3){
                        temp[entry] = key;
                        entry++;
                        }
                        else if (entry == 3) {
                            temp[entry] = key;
                            entry = 0;                            
                            if (checkPass() == 1) {
                                clearLCD();
                                moveCursorLCD(1);
                                printStringLCD("Good Password!");
                                for (i = 0; i < 400; i++){
                                delayUs(5000);
                                }
                                state = enter;
                            }
                            else {
                                moveCursorLCD(1);
                                printStringLCD("BAD Password");
                                moveCursorLCD(0);
                                printStringLCD("Destruct in 5...");
                                for (i = 0; i < 400; i++){
                                delayUs(5000);
                                }
                                state = enter;
                            }
                        }
                    }

                    break;

                
                case entryWait:
                    break;

                 
                case setMode:
                    moveCursorLCD(0);
                    key = scanKeypad();
                    printCharLCD(key);
                    state = setWait;
                                        
                    
                    if (key == '*') {
                        star++;
                        entry = 0;
                        if (star == 2) {
                            state = setWait;
                            moveCursorLCD(1);
                            clearLCD();
                            printStringLCD("Set Mode");
                            star = 0;
                        }
                        else if (star == 1 && (key != '*' )) {
                            entry = 0;
                            star = 0;
                            moveCursorLCD(1);
                            printStringLCD("Bad Entry");
                            state = enter;
                        }
                    }
                    else if (key == '#') {
                        entry = 0;
                        clearLCD();
                        moveCursorLCD(1);
                        printStringLCD("Bad Entry");
                        state = enter;
                    }
                    
                    else {
                        star = 0;
                        if (entry < 3) {
                        temp[entry] = key;
                        entry++;
                        }
                        else if (entry == 3) {
                            temp[entry] = key;
                            entry = 0;
                            storePass();
                            state = enter;
                        }
                    }
                    
                    
                    break;
                    
                case setWait: clearLCD();
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
        if (PORTC1 == 0 || PORTC2 == 0) {
            if (state == wait || state == entryWait) {
              nextState = entryMode;
              state = debounce; 
            }
            else if (state == setMode || state == setWait) {
              nextState = setMode;
              state = debounce;
            }
        }

      }
                     
        if (PORTC1 == 1) {
            nextState = state;
            state = debounce;
        }    
        if (PORTC2 == 1) {
            nextState = state;
            state = debounce;
        } 
  

    else if (IFS1bits.CNCIF == 1) {
        IFS1bits.CNCIF = OFF;
        if (PORTC3 == 0) {
            if (state == wait || state == entryWait) {
              nextState = entryMode;
              state = debounce; 
            }
            else if (state == setMode || state == setWait) {
              nextState = setMode;
              state = debounce;
            }
        }
    }
         if (PORTC3 == 1) {
            nextState = state;
            state = debounce;
        }
    
  }


void initPasswords() {
    
    int i = 0;
    int j = 0;
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            passwords[i][j] = 'x';
        }
    }
}

void storePass() {
    int i = 0;
    
    for (i = 0; i < 4; i++) {
        passwords[currRow][i] = temp[i];
    }
    currRow++;
    if (currRow == 3) {
        currRow = 0;
    }
}
    
int checkPass() {
    int i = 0;
    int j = 0;
    int k = 0;
    
    for (i = 0; i < 4; i++) {
        k = 0;
        for (j = 0; j < 4; j++) {
            if (temp[j] == passwords[i][j]) {
                k++;
            }
        }
        
        if (k == 3) {
            return 1;
        }
        
    }
    
    return 0;
}
