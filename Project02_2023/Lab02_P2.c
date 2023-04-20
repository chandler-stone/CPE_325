/*----------------------------------------------------------------------------------
 * File:        Lab02_P2.c
 * Description: This program finds the average value per index of two input arrays.
 *
 * Board:       5529
 * Input:       Two hard-coded integer arrays.
 * Output:      Float array of average values per index of two input arrays printed to console.
 * Author:      Chandler Stone
 * Date:        January 26, 2023
 *----------------------------------------------------------------------------*/
#include <msp430.h>
#include <stdio.h>

int main(void) {
    WDTCTL = WDTPW + WDTHOLD; // stops watchdog timer.

    int X[5] = {-1,2,5,3,5};
    const int m = 5;
    const int c = -2;
    int Y[5];
    unsigned int i = 0;
    static unsigned int max = sizeof(X) / sizeof(int);

    // computing y values
    while(i < max) {
        Y[i] = m*X[i] + c;
        i++;
    }

    //printing input array X.
    i = 0;
    printf("Input Array X: [");
    while(i < max) {
        printf("%i", X[i]);
        if(i < max - 1)
            printf(" ");
        i++;
    }

    //printing input constants.
    printf("]\nInput Constants m: %i and c: %i", m, c);

    //printing output array Y.
    printf("\nOutput Array Y: [");
    i = 0;
    while(i < max) {
        printf("%i", Y[i]);
        if(i < max - 1)
            printf(" ");
        i++;
    }
    printf("]");
    return 0;
}
