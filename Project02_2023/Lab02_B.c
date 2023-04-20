/*----------------------------------------------------------------------------------
 * File:        Lab02_B.c
 * Description: This program finds the matrix multiplication of two input matrices.
 *
 * Board:       5529
 * Input:       Two hard-coded integer matrices.
 * Output:      Integer resultant matrix multiplication of two input matrices printed to console.
 * Author:      Chandler Stone
 * Date:        January 27, 2023
 *----------------------------------------------------------------------------*/
#include <msp430.h>
#include <stdio.h>

int main(void) {
    WDTCTL = WDTPW + WDTHOLD; // stops watchdog timer.

    int x[8][8] = {{1,0,5,0,0,0,0,9},
                   {0,1,0,0,0,6,0,0},
                   {0,0,1,3,0,0,0,0},
                   {0,0,0,1,0,0,0,0},
                   {0,0,0,8,1,0,0,6},
                   {0,0,0,0,0,1,0,0},
                   {0,0,0,0,0,0,1,0},
                   {0,0,3,0,0,0,0,1}};

    int y[8][8] = {{5,5,5,5,5,5,5,5},
                   {0,0,0,0,0,5,0,7},
                   {0,0,8,0,0,2,0,0},
                   {0,0,0,3,0,2,0,0},
                   {0,0,0,0,0,0,0,0},
                   {0,0,0,0,6,0,0,0},
                   {0,0,0,0,0,9,0,0},
                   {7,0,0,0,0,1,0,0}};

    int z[8][8] = {};
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int k = 0;
    int sum = 0;

    //calculating matrix product.
    while(i < 8) {
        while(j < 8) {
            while(k < 8) {
                sum += x[i][k] * y[k][j];
                k++;
            }
            z[i][j] = sum;
            sum = 0;
            k = 0;
            j++;
        }
        j = 0;
        i++;
    }

    //printing input matrix x.
    i = 0;
    printf("Input Matrix x:  ");
    while(i < 8) {
        while(j < 8) {
            printf("%i", x[i][j]);
            if(j < 7)
                printf(" ");
            j++;
        }
        j = 0;
        if(i < 7)
            printf("\n                 ");
        i++;
    }

    //printing input matrix y.
    i = 0;
    printf("\n\nInput Matrix y:  ");
    while(i < 8) {
        while(j < 8) {
            printf("%i", y[i][j]);
            if(j < 7)
                printf(" ");
            j++;
        }
        j = 0;
        if(i < 7)
            printf("\n                 ");
        i++;
    }

    //printing output matrix z.
    i = 0;
    printf("\n\nOutput Matrix z: ");
    while(i < 8) {
        while(j < 8) {
            if(z[i][j] / 10 > 1) {
                printf("0");
            }
            else {
                printf("00");
            }
            printf("%i", z[i][j]);
            if(j < 7)
                printf(" ");
            j++;
        }
        j = 0;
        if(i < 7)
            printf("\n                 ");
        i++;
    }
    return 0;
}
