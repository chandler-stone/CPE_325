/*----------------------------------------------------------------------------------
 * File:        Lab02_P1.c
 * Description: This program prints out the number of strings that have .
 *
 * Board:       MSP430F5529
 * Input:       No inputs.
 * Output:      Table of C data types with their sizes, minimum values, and maximum values.
 * Author:      Chandler Stone
 * Date:        August 30, 2022
 *----------------------------------------------------------------------------*/

#include <msp430.h>
#include <stdio.h>

/**
 * main.c
 */

const char input1[100] = "This is another sample input. There is a conspiracy.";
const char input2[100] = "This is the sample input. There is no conspiracy.";

char stringCharTotal[];


int countLetters(const char* str) {
    unsigned int i = 0;
    unsigned int j = 0;
    int charTotal = 0;
    int stringLineDivisibleTotal = 0;
    while (str[i] != 0x00) {
        if (((str[i] >= 'A') && (str[i] <= 'Z')) || ((str[i] > 'a') && (str[i] < 'z'))) {
            charTotal++;
        }
        else {
            stringCharTotal[j] = charTotal;
            charTotal = 0;
            j++;
        }
        i++;
    }
    stringCharTotal[j] = 0x00;
    i = 0;
    while (str[i] != 0x00) {
        if (stringCharTotal[i] == stringCharTotal[i+1]) {
            stringLineDivisibleTotal++;
        }
        i++;
    }
    return stringLineDivisibleTotal;
}

void printOutput(int out) {
    printf("output for string: %d", out);
}

void main() {
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	int out = countLetters(input1);
	printOutput(out);
}

