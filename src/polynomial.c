#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include "polynomial.h"

void analogNum(double complex *poles, uint16_t order, double complex *numerator) {
    numerator[0] = poles[0];

    for (uint32_t i = 1; i < order; i++) {
        numerator[0] *= poles[i];
    }

    return;
}

// The double complex *denominator array should have length order + 1
void analogDen(double complex *poles, uint16_t order, double complex *denominator) {

    uint8_t  numArrays = 2;
    uint8_t  oddOrder  = order % 2;
    uint32_t numPoly   = order + oddOrder;
    uint32_t polyDepth = numPoly + 1;

    uint32_t polySizeAlloc = (2 * order + 1 ) * sizeof(double complex);
    uint32_t polySize      = (2 * order + 1 );

    // allocate memory to store the first order polynomials in
    double complex ***polynomials = malloc(numArrays * sizeof(double complex **));
    if (polynomials == NULL) {
        printf("Not enough memory\n");
        exit(EXIT_FAILURE);
    }
    for (uint16_t i = 0; i < numArrays; i++) {
        polynomials[i] = malloc(numPoly * sizeof(double complex *));
        if (polynomials[i] == NULL) {
            printf("Not enough memory\n");
            exit(EXIT_FAILURE);
        }
        for (uint32_t j = 0; j < polyDepth; j++) {
            polynomials[i][j] = malloc(polySizeAlloc);
            if (polynomials[i][j] == NULL) {
                printf("Not enough memory\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Fill the first order polynomials (1s-Pi)
    for (uint32_t i = 0; i < order; i++) {
        polynomials[0][i][0] = poles[i];
        polynomials[0][i][1] = 0 + 0 * I;

        // Set the rest of the polynomial to 0+0j
        for (uint32_t j = 2; j < polySize; j++) {
            polynomials[0][i][j] = 0 + 0 * I;
        }

        // Set the second polynomial array completlly to 0
        for (uint32_t j = 0; j < polySize; j++) {
            polynomials[1][i][j] = 0 + 0 * I;
        }
    }

        


    for (uint32_t i = 0; i <= (order + oddOrder); i++) {

    }


    // Free memory and return
    for (uint32_t i = 0; i < order + 1; i++) {
        free(polynomials[i]);
    }
    free(polynomials);

    return;
}

// The double complex *numerator array should have length order + 1
void digitalNum(double complex *poles, uint16_t order, double complex *numerator) {

    return;
}

// The double complex *denominator array should have length order + 1
void digitalDen(double complex *poles, uint16_t order, double complex *denominator) {

    return;
}


void polyMul(double complex *poly1, double complex *poly2, uint16_t degree, double complex *polyRes) {
    uint32_t elementsPolyIn  = degree + 1;

    // Set polyRes to zero
    for (uint32_t i = 0; i < degree * 2 + 1; i++) {
        polyRes[i] = 0 + 0 * I;
    }

    // Calculate the result of the multiplication of poly1 with poly2
    for (uint32_t elementPoly1 = 0; elementPoly1 < elementsPolyIn; elementPoly1++) {
        for (uint32_t elementPoly2 = 0; elementPoly2 < elementsPolyIn; elementPoly2++) {
            polyRes[elementPoly1 + elementPoly2] += poly1[elementPoly1] * poly2[elementPoly2];
        }
    }
    
    return;
}

void examplePolyMul(void) {
    
    double complex poly1[3]   = {3 + 0 * I, 2 + 0 * I, 1 + 0 * I};
    double complex poly2[3]   = {6 + 0 * I, 5 + 0 * I, 4 + 0 * I};
    double complex polyRes[5] = {6 + 0 * I,
                                 5 + 0 * I,
                                 4 + 0 * I, 
                                 3 + 0 * I,
                                 2 + 0 * I};

    polyMul(poly1, poly2, 2, polyRes);

    printf("polyRes af polyMul 2 =");
    for (uint32_t i = 0; i < 5; i++) {
        printf("%.2f+%.2fj%s", creal(polyRes[i]), cimag(polyRes[i]), i == 5 - 1 ? "": " | ");
    }
    printf("\n");

    printf("polyRes=");
    for (int16_t i = 4; i >= 0; i--) {
        printf("%.2fx^%d%s", creal(polyRes[i]), i, i == 0 ? "": " + ");
    }
    printf("\ntest\n");    

    return;
}