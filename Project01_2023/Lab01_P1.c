/*----------------------------------------------------------------------------------
 * File:        Lab01_P1.c
 * Description: This program preforms exponentiation on a hardcoded integer, a, to the power of another hardcoded integer, p, the result is printed to the console.
 *
 * Board:       5529
 * Input:       Two hardcoded integers.
 * Output:      "a to the power of p is b", where a and p are hardcoded values and b is the exponential result.
 * Author:      Chandler Stone
 * Date:        January 13, 2023
 *----------------------------------------------------------------------------*/
#include <msp430.h>
#include <stdio.h>

long int calcPower(int base, int exponent) {
    long int result = 1;
    int i;
    for (i = 0; i < exponent; i++) {
            result *= base;
    }
    return result;
}

void main(void) {
     WDTCTL = WDTPW + WDTHOLD; // stops watchdog timer.

     int a = 13;
     int p = 5;
     long int b = calcPower(a, p);

     printf("%i raised to the power %i is %li", a, p, b);
}
