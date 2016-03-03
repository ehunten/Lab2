/*
 * File:   password.h
 * Authors: Evelyn Hunten
 *
 * Created 3/2/16
 */

char *initPassArrays();
void setPass(char pass);
void storePass(char *temp, char *passwords);
void readPass (char pass, int pos);
int checkPass(char *temp, char *passwords);