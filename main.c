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
    enter, open, wait, dbEnter, dbSet, setMode, entryWait, entryMode, setWait, entryOpen, setOpen
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
    initTemp();

    while (1) {

             
            switch (state) {
                case enter:
                    moveCursorLCD(1);
                    printStringLCD("Entry Mode");
                    moveCursorLCD(0);
                    state = wait;
                    break;

                case wait:
                    break;

                case dbEnter:
                    state = entryMode;
                    delayUs(50);
                    break;
                    
                case dbSet:
                    state = setMode;
                    delayUs(50);
                    break;

                case entryMode:
                    clearLCD();
                    moveCursorLCD(1);
                    printStringLCD("Entry Mode");
                    key = scanKeypad();
                    moveCursorLCD(0);
                    printCharLCD(key);
                    state = entryWait;

                    if (star == 1 && (key != '*' )) {
                            star = 0;
                            moveCursorLCD(1);
                            printStringLCD("Bad Entry");
                            //break;
                        }
                    
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
                    }
                    else if (key == '#') {
                        entry = 0;
                        clearLCD();
                        moveCursorLCD(1);
                        printStringLCD("Bad Entry");
                        for (i = 0; i < 400; i++){
                        delayUs(5000);
                        }
                        break;
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
                                clearLCD();
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
                                clearLCD();
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
                                        
                    if (star == 1 && (key != '*' )) {
                        entry = 0;
                        star = 0;
                        moveCursorLCD(1);
                        printStringLCD("Bad Entry");
                        state = enter;
                        clearLCD();
                    }
                    
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

                    }
                    else if (key == '#') {
                        entry = 0;
                        clearLCD();
                        moveCursorLCD(1);
                        printStringLCD("Bad Entry");
                        state = enter;
                        clearLCD();
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
                            clearLCD();
                        }
                    }
                    
                    
                    break;
                    
                case setWait:
                    break;

                default: printStringLCD("Ruh roh!");
                    break;
            }
    }


    return 0;
}
//*********************
void initPasswords() {
    
    int i = 0;
    int j = 0;
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            passwords[i][j] = '1';
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

    return;
}
    
int checkPass() {
    int i = 0;
    int j = 0;
    int same = 0;
    
    
    for (i = 0; i < 4; i++) {
        same = 0;
        for (j = 0; j < 4; j++) {
            if (temp[j] == passwords[i][j]) {
                same++;
            }
            if (same == 3) {
                return 1;
            }            
        }
    
    }
    
    return 0;
}

void initTemp() {
    int i = 0;
    
    for (i = 0; i < 4; i++){
        temp[i] = 'x';
    }
}




//**********************

//Button Interrupt
void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void){

    PORTD;
    PORTC;

    if (IFS1bits.CNDIF == 1){
        IFS1bits.CNDIF = OFF;
        if (PORTC1 == 0 || PORTC2 == 0) {
            if (state == wait || state == entryWait) {
              state = dbEnter; 
            }
            else if (state == setMode || state == setWait) {
              state = dbSet;
            }
        }

      }
                     
        if (PORTC1 == 1) {

        }    
        if (PORTC2 == 1) {

        } 
  

    if (IFS1bits.CNCIF == 1) {
        IFS1bits.CNCIF = OFF;
        if (PORTC3 == 0) {
            if (state == wait || state == entryWait) {
              state = dbEnter; 
            }
            else if (state == setMode || state == setWait) {
              state = dbSet;
            }
        }
    
  }
    if (PORTC3 == 1) {
 
    }
    
}


