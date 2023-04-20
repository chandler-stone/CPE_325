/*----------------------------------------------------------------------------------
 * File:        Lab02_P3.c
 * Description: This program finds the average value per index of two input arrays.
 *
 * Board:       5529
 * Input:       Two hard-coded integer arrays.
 * Output:      Float array of average values per index of two input arrays printed to console.
 * Author:      Chandler Stone
 * Date:        August 30, 2022
 *----------------------------------------------------------------------------*/
#include <msp430.h>
#include <stdio.h>

int main(void) {
    WDTCTL = WDTPW + WDTHOLD; // stops watchdog timer.

    int x[5] = {-20,5,0,3,-53};
    int y[5] = {-32,5,2,-22,7};
    float z[5];
    unsigned int i = 0;
    static unsigned int max = sizeof(z) / sizeof(float);

    //calculations for average values
    while(i < max) {
        z[i] = ((float)x[i] + (float)y[i]) / 2.0;
        i++;
    }

    //printing input array x.
    i = 0;
    printf("Input Array x: [");
    while(i < max) {
        printf("%i", x[i]);
        if(i < max - 1)
            printf(" ");
        i++;
    }

    //printing input array y.
    printf("]\nInput Array y: [");
    i = 0;
    while(i < max) {
        printf("%i", y[i]);
        if(i < max - 1)
            printf(" ");
        i++;
    }

    //printing output array z.
    printf("]\nOutput Array z: [");
    i = 0;
    while(i < max) {
        printf("%.2f", z[i]);
        if(i < max - 1)
            printf(" ");
        i++;
    }
    printf("]");
    return 0;
}
