// ******************************************************************************************* //
//
// File:         password.c
// Date:         3/2/16
// Authors:      Evelyn Hunten
//
// Description: Part 2 for lab 2
// ******************************************************************************************* //

#include <xc.h>
#include "password.h"

//declare global char array
char password[4];

void setPass(char pass) {
    
}


void storePass(char pass) {

    
    return;
}


void readPass(char key) {
    int i = 0;
    
    
    //initialize password char array
    for(i = 0; i < 4; i++)
    {
        password[i] = 'x';
    }
    
    //set key to password array
    for(i = 0; i < 4; i++)
    {
        if(password[i] == 'x')
        {
            password[i] = key;
            break;
        }
    }
    
    return;
}


int checkPass(char pass) {
    
    
}