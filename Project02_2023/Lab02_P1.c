/*----------------------------------------------------------------------------------
 * File:        Lab02_P1.c
 * Description: This program prints out a chart of C data types with their sizes, minimum values, and maximum values.
 *              The Data types are pulled from their local hardware to be perfectly accurate to the hardware they are on.
 *
 * Board:       5529
 * Input:       No inputs.
 * Output:      Table of C data types with their sizes, minimum values, and maximum values.
 * Author:      Chandler Stone
 * Date:        January 26, 2023
 *----------------------------------------------------------------------------*/
#include <msp430.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>

int main (void) {
    WDTCTL = WDTPW + WDTHOLD; // stops watchdog timer.

    printf("---------------------------------------------------------------------------------------------\n");
    printf("| Data Type              | Size  (in bytes)  | Minimum               | Maximum              |\n");
    printf("---------------------------------------------------------------------------------------------\n");
    printf("| char                   | %i                 | %i                  | %i                  |\n", sizeof(char), SCHAR_MIN, SCHAR_MAX);
    printf("| float                  | %i                 | %e          | %e         |\n", sizeof(float), FLT_MIN, FLT_MAX);
    printf("| int                    | %i                 | %i                | %i                |\n", sizeof(int), INT_MIN, INT_MAX);
    printf("| long int               | %i                 | %li           | %li           |\n", sizeof(long int), LONG_MIN, LONG_MAX);
    printf("| unsigned long int      | %i                 | %i                     | %lu           |\n", sizeof(unsigned long int), 0, ULONG_MAX);
    printf("| unsigned char          | %i                 | %i                     | %i                  |\n", sizeof(unsigned char), 0, UCHAR_MAX);
    printf("| unsigned short int     | %i                 | %i                     | %hu                |\n", sizeof(unsigned short int), 0, USHRT_MAX);
    printf("| unsigned int           | %i                 | %i                     | %u                |\n", sizeof(unsigned int), 0, UINT_MAX);
    printf("| long long int          | %i                 | %lli  | %lli  |\n", sizeof(long long int), LLONG_MIN, LLONG_MAX);
    printf("| unsigned long long int | %i                 | %i                     | %llu |\n", sizeof(unsigned long long int), 0, ULLONG_MAX);
    printf("| short int              | %i                 | %hi                | %hi                |\n", sizeof(short int), SHRT_MIN, SHRT_MAX);
    printf("| double                 | %i                 | %e         | %e        |\n", sizeof(double), DBL_MIN, DBL_MAX);
    printf("---------------------------------------------------------------------------------------------\n");
}
