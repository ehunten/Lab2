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

#define TRISG13 TRISGbits.TRISG13   
#define TRISG0 TRISGbits.TRISG0
#define TRISF1 TRISFbits.TRISF1
#define TRISG12 TRISGbits.TRISG12
#define TRISD5 TRISDbits.TRISD5
#define TRISD11 TRISDbits.TRISD11
#define TRISC14 TRISCbits.TRISC14

#define ODCG13 ODCGbits.ODCG13
#define ODCG0 ODCGbits.ODCG0
#define ODCF1 ODCFbits.ODCF1
#define ODCG12 ODCGbits.ODCG12

#define LATG13 LATGbits.LATG13
#define LATG0 LATGbits.LATG0
#define LATF1 LATFbits.LATF1
#define LATG12 LATGbits.LATG12

#define PORTD5 PORTDbits.RD5
#define PORTD11 PORTDbits.RD11
#define PORTC14 PORTCbits.RC14


#define OUT 0
#define ENABLE 0
#define IN 1
/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
void initKeypad(void){
    
    //set rows to outputs
    TRISG13     = OUT;
    TRISG0      = OUT;
    TRISF1      = OUT;
    TRISG12     = OUT;
    
    //set columns to inputs
    TRISD5      = IN;
    TRISD11     = IN;
    TRISC14     = IN;
    
    //enable open drain collection for rows
    ODCG13      = ENABLE;
    ODCG0       = ENABLE;
    ODCF1       = ENABLE;
    ODCG12      = ENABLE;
    
    

    
}

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed. The ascii character c programmatically is just 'c'
 */
char scanKeypad(void){
    char key = -1;
    
    
    
    return key;
}