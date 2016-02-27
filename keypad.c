// ******************************************************************************************* //
//
// File:         keypad.c
// Date:         2/25/16
// Authors:      Evelyn Hunten
//
// Description: Part 1 for lab 2
// ******************************************************************************************* //

#include <xc.h>
#include "keypad.h"
#include "timer.h"
//rows
#define TRISR1 TRISGbits.TRISG13
#define TRISR2 TRISGbits.TRISG0
#define TRISR3 TRISFbits.TRISF1
#define TRISR4 TRISGbits.TRISG12
//columns
#define TRISC1 TRISDbits.TRISD5
#define TRISC2 TRISDbits.TRISD11
#define TRISC3 TRISCbits.TRISC14

#define ODCR1 ODCGbits.ODCG13
#define ODCR2 ODCGbits.ODCG0
#define ODCR3 ODCFbits.ODCF1
#define ODCR4 ODCGbits.ODCG12

#define PORTC1 PORTDbits.RD5
#define PORTC2 PORTDbits.RD11
#define PORTC3 PORTCbits.RC14

#define LATR1 LATGbits.LATG13
#define LATR2 LATGbits.LATG0
#define LATR3 LATFbits.LATF1
#define LATR4 LATGbits.LATG12

#define OUT 0
#define ENABLE 1
#define DISABLE 0
#define IN 1

typedef enum odc_rowEnum{
    row1, row2, row3, row4
} odc_row;

volatile stateType odc_row = row1;

/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
void initKeypad(void){

    //set rows to outputs
    TRISR1      = OUT;
    TRISR2      = OUT;
    TRISR3      = OUT;
    TRISR4      = OUT;

    //set columns to inputs
    TRISC1      = IN;
    TRISC2      = IN;
    TRISC3      = IN;
    
    //Enable pull-up resistors for columns
    CNPUDbits.CNPUD5  = ENABLE;
    CNPUDbits.CNPUD11 = ENABLE;
    CNPUCbits.CNPUC14 = ENABLE;

    //enable open drain collection for rows
    ODCR1       = ENABLE;
    ODCR2       = ENABLE;
    ODCR3       = ENABLE;
    ODCR4       = ENABLE;

}

/*This function gives the controller a state where it can wait for a
 *key to be pressed
 */
void openScanning (void) {
    ODCR1       = ENABLE;
    ODCR2       = ENABLE;
    ODCR3       = ENABLE;
    ODCR4       = ENABLE;
}
/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed. The ascii character c programmatically is just 'c'
 */
char scanKeypad(void){
    char key = -1;

    switch (odc_row) {
     case row1:
     ODCR1       = 0;
     ODCR2       = 1;
     ODCR3       = 1;
     ODCR4       = 1;
       if (PORTC1 == 0) { while (PORTC1 == 0) { return '1'; }}
       else if (PORTC2 == 0) { while (PORTC2 == 0) { return '2'; }}
       else if (PORTC3 == 0) { while (PORTC3 == 0) { return '3'; }}
     odc_row = row2;
     break;

     case row2:
     ODCR1       = 1;
     ODCR2       = 0;
     ODCR3       = 1;
      if (PORTC1 == 0) { while (PORTC1 == 0) { return '4'; }}
       else if (PORTC2 == 0) { while (PORTC2 == 0) { return '5'; }}
       else if (PORTC3 == 0) { while (PORTC3 == 0) { return '6'; }}
     ODCR4       = 1;
     odc_row = row3;
     break;

     case row3:
     ODCR1       = 1;
     ODCR2       = 1;
     ODCR3       = 0;
     ODCR4       = 1;
      if (PORTC1 == 0) { while (PORTC1 == 0) { return '7'; }}
       else if (PORTC2 == 0) { while (PORTC2 == 0) { return '8'; }}
       else if (PORTC3 == 0) { while (PORTC3 == 0) { return '9'; }}
     odc_row = row4;
     break;

     case row4:
     ODCR1       = 1;
     ODCR2       = 1;
     ODCR3       = 1;
     ODCR4       = 0;
       if (PORTC1 == 0) { while (PORTC1 == 0) { return '*'; }}
       else if (PORTC2 == 0) { while (PORTC2 == 0) { return '0'; }}
       else if (PORTC3 == 0) { while (PORTC3 == 0) { return '#'; }}
     odc_row = row1;
     break;
    }

    return key;
}
