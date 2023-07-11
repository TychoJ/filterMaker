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

    uint32_t polySizeNeeded = (2 * order + 1 ) * sizeof(double complex);
    uint8_t  oddOrder = order % 2;

    // allocate memory to store the first order polynomials in
    double complex **polynomials = malloc((order + 1) * sizeof(double complex));
    if (polynomials == NULL) {
        printf("Not enough memory\n");
        exit(EXIT_FAILURE);
    }
    for (uint32_t i = 0; i < order + 1; i++) {
        polynomials[i] = malloc(polySizeNeeded);
        if (polynomials[i] == NULL) {
            printf("Not enough memory\n");
            exit(EXIT_FAILURE);
        }
    }

    // Fill the first oreder polynomials (1s-Pi)
    for (uint32_t i = 0; i < order; i++) {
        polynomials[i][0] = poles[i];
        polynomials[i][1] = 0 + 0 * I;

        for (uint32_t j = 2; j < 2 * order + 1; j++) {
            polynomials[i][j] = 0 + 0 * I;
        }
    }
    for (uint32_t j = 2; j < 2 * order + 1; j++) {
        polynomials[order][j] = 0 + 0 * I;
    }
    polynomials[order][2*order] = 1 + 0 * I;


    for (uint32_t i = 0; i <= (order + oddOrder); i++) {

    }




    //-----------------------------------------------------------------------------------------------------------------------

    // Create a temporary polynomial 
    double complex *tmpPoly = malloc(polySizeNeeded);
    if (tmpPoly == NULL) {
        printf("Not enough memory\n");
        exit(EXIT_FAILURE);
    }

    // Check if order is larger then 1
    if (order == 1) {
        denominator[0] = poles[0];
        denominator[1] = 0 + 0 * I;

        // Free memory and return
        for (uint32_t i = 0; i < order + 1; i++) {
            free(polynomials[i]);
        }
        free(polynomials);
        free(tmpPoly);
        return;
    }

    // Calculate the resulting polynomial
    polyMul(polynomials[0], polynomials[1], 1, tmpPoly);
   // polyMul(tmpPoly, polynomials[2], polynomials[0]);

    for (uint32_t i = 0; i < order; i++) {
        polyMul(polynomials[i], polynomials[i + 1], i, polynomials[(i + order)%(order + 1)]);
    }


    // Free memory and return
    for (uint32_t i = 0; i < order + 1; i++) {
        free(polynomials[i]);
    }
    free(polynomials);
    free(tmpPoly);

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

    printf("a b c d e f g h i j k l m n o p q r s t u v w x y z\n");

    // Set polyRes to zero
    for (uint32_t i = 0; i < degree * 2 + 1; i++) {
        polyRes[i] = 0 + 0 * I;
    }
    printf("can you show this please!!!\n");


    printf("calculate stuff:\n");
    // Calculate the result of the multiplication of poly1 with poly2
    for (uint32_t elementPoly1 = 0; elementPoly1 < elementsPolyIn; elementPoly1++) {
        printf("poly1[%d]\n", elementPoly1);
        for (uint32_t elementPoly2 = 0; elementPoly2 < elementsPolyIn; elementPoly2++) {
            printf("\tpoly2[%d]\n\tpolyRes[%d]= %.2f+%.2fj + [%.2f+%.2fj]*[%.2f+%.2fj]",
                elementPoly2,
                elementPoly1 + elementPoly2,
                creal(polyRes[elementPoly1 + elementPoly2]),
                cimag(polyRes[elementPoly1 + elementPoly2]),
                creal(poly1[elementPoly1]),
                cimag(poly1[elementPoly1]),
                creal(poly2[elementPoly2]),
                cimag(poly2[elementPoly2]));
            polyRes[elementPoly1 + elementPoly2] += poly1[elementPoly1] * poly2[elementPoly2];
            printf(" = %0.2f+%0.2fj\n",
                creal(polyRes[elementPoly1 + elementPoly2]),
                cimag(polyRes[elementPoly1 + elementPoly2]));
        }
    }
    printf("\n");
    
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
    printf("say hi please!!!\n");
    printf("array length of poly1   = %ld\n", sizeof(poly1)/sizeof(double complex));
    printf("array length of poly2   = %ld\n", sizeof(poly2)/sizeof(double complex));
    printf("array length of polyRes = %ld\n", sizeof(polyRes)/sizeof(double complex));
    printf("hello?\n");

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