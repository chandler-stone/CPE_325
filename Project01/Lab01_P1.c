/*----------------------------------------------------------------------------------
 * File:        Lab01_P1.c
 * Description: This program preforms a Ceaser cipher on a hardcoded message.
 *
 * Board:       5529
 * Input:       Hardcoded character array message.
 * Output:      message with 3 shift Ceaser cipher. (not on non-alphabet characters)
 * Author:      Chandler Stone
 * Date:        August 25, 2022
 *----------------------------------------------------------------------------*/
#include <msp430.h>
#include <stdio.h>

char msg[] = "Welcome to CPE325 Fall 2022!";

 cipher(char message[]) {
    int i = sizeof(message) / sizeof(char);
    char cipheredMessage[] = {};

    while(i > 0) {
        cipheredMessage[i] = (int)message[i] + 3;
        i--;
    }
    return cipheredMessage;
}

 //printf(“%s\n”, msg);
