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
#define TRISR4 TRISGbits.TRISG0
#define TRISR3 TRISFbits.TRISF1
#define TRISR2 TRISDbits.TRISD12

//columns
#define TRISC1 TRISDbits.TRISD5
#define TRISC2 TRISDbits.TRISD11
#define TRISC3 TRISCbits.TRISC14

#define ODCR1 ODCGbits.ODCG13
#define ODCR4 ODCGbits.ODCG0
#define ODCR3 ODCFbits.ODCF1
#define ODCR2 ODCDbits.ODCD12

#define PORTC1 PORTDbits.RD5
#define PORTC2 PORTDbits.RD11
#define PORTC3 PORTCbits.RC14

#define LATR1 LATGbits.LATG13
#define LATR4 LATGbits.LATG0
#define LATR3 LATFbits.LATF1
#define LATR2 LATDbits.LATD12

#define CNPUC1 CNPUDbits.CNPUD5 
#define CNPUC2 CNPUDbits.CNPUD11 
#define CNPUC3 CNPUCbits.CNPUC14 
    
#define CNENC1 CNENDbits.CNIED5 
#define CNENC2 CNENDbits.CNIED11 
#define CNENC3 CNENCbits.CNIEC14 

#define OUT 0
#define ENABLE 1
#define DISABLE 0
#define IN 1
#define CLOSED 1
#define OPEN 0

typedef enum odc_row_stateEnum{
    row1, row2, row3, row4
} odc_row_state;

volatile  odc_row_state odc_row = row1;

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
     
    
    //Disable pull-up resistors for columns
    CNPUC1      = DISABLE;
    CNPUC2      = DISABLE;
    CNPUC3      = DISABLE;
    
    //Enable change notifications for columns
    CNENC1      = ENABLE;
    CNENC2      = ENABLE;
    CNENC3      = ENABLE;
    
    IPC8bits.CNIP = 7;
    IFS1bits.CNDIF = 0;
    IFS1bits.CNCIF = 0;
    IEC1bits.CNDIE = 1;     //Enable interrupt
    IEC1bits.CNCIE = 1;     //Enable interrupt
    
    CNCONDbits.ON = 1;
    CNCONCbits.ON = 1;
    

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

    LATR1       = OPEN;
    LATR2       = OPEN;
    LATR3       = OPEN;
    LATR4       = OPEN;
    
}
/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed. The ascii character c programmatically is just 'c'
 */
char scanKeypad(void){
    char key = 'Q';
      
IEC1bits.CNCIE = 0;
IEC1bits.CNDIE = 0;
//checking first row
        LATR1       = OPEN;
        LATR2       = CLOSED;
        LATR3       = CLOSED;
        LATR4       = CLOSED;
        delayUs(50);
        if (PORTC1 == 0)  { key =  '1'; } 
           else if (PORTC2 == 0)  { key = '2'; }
           else if (PORTC3 == 0)  { key = '3'; }
           
        
//checking second row
        LATR1       = CLOSED;
        LATR2       = OPEN;
        LATR3       = CLOSED;
        LATR4       = CLOSED;
        delayUs(50);
        if (PORTC1 == 0)  { key = '4'; }
        else if (PORTC2 == 0) { key = '5'; }
        else if (PORTC3 == 0) { key = '6'; }
   
       
  //checking third row
        LATR1       = CLOSED;
        LATR2       = CLOSED;
        LATR3       = OPEN;
        LATR4       = CLOSED;
        delayUs(50);
          if (PORTC1 == 0) { key = '7'; }
          else if (PORTC2 == 0) { key = '8'; }
          else if (PORTC3 == 0) { key = '9'; }
        
    
 //checking fourth row
        LATR1       = CLOSED;
        LATR2       = CLOSED;
        LATR3       = CLOSED;
        LATR4       = OPEN;
        delayUs(50);
           if (PORTC1 == 0) { key = '*'; }
           else if (PORTC2 == 0) { key = '0'; }
           else if (PORTC3 == 0) { key = '#'; }
           
       
        LATR1       = CLOSED;
        LATR2       = CLOSED;
        LATR3       = CLOSED;
        LATR4       = CLOSED;
       openScanning();
       IEC1bits.CNCIE = 1;
       IEC1bits.CNDIE = 1;
    
    return key;
}
