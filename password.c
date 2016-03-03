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

volatile int currRow = 0;

char *initPassArrays(){
    char *passwords[4][4];
    int i = 0;
    int j = 0;
    char *pass;
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            *passwords[i][j] = 'x';
        }
        }
    pass = passwords;
    
    return pass;
}


void setPass(char pass) {
    
}




void storePass(char *temp, char *passwords) {
    int i = 0;
    
    for (i = 0; i < 4; i++) {
        passwords[currRow][i] = temp[i];
    }
    currRow++;
    if (currRow == 3) {
        currRow = 0;
    }
}


void readPass(char pass, int pos) {
    if (pos == 0) {
        //store in position 0 in array
    }
}


int checkPass(char *temp, char *passwords) {
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