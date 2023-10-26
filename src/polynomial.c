#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include <math.h>
#include "polynomial.h"

static void combinePoly(double complex ***polynomials, uint16_t order, double complex *ret);

void analogNum(double complex *poles, uint16_t order, double complex *numerator) {
    numerator[0] = poles[0];

    for (uint32_t i = 1; i < order; i++) {
        numerator[0] *= poles[i];
    }

    return;
}

// The double complex *denominator array should have length: order + 1
void analogDen(double complex *poles, uint16_t order, double complex *denominator) {

    printf("starting analogDen\n");

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
        for (uint32_t j = 0; j < numPoly; j++) {
            polynomials[i][j] = malloc(polySizeAlloc);
            if (polynomials[i][j] == NULL) {
                printf("Not enough memory\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    
    printf("Filling the first order polynomials in the array's\n");
    // Fill the first order polynomials in array 0 (1s-Pi)
    for (uint32_t i = 0; i < order; i++) {
        polynomials[0][i][0] = -1 * poles[i];
        polynomials[0][i][1] = 1 + 0 * I;

        // Set the rest of the polynomial to 0+0j
        for (uint32_t j = 2; j < polySize; j++) {
            polynomials[0][i][j] = 0 + 0 * I;
        }

        // Set the second polynomial array completlly to 0
        for (uint32_t j = 0; j < polySize; j++) {
            polynomials[1][i][j] = 0 + 0 * I;
        }
    }

    // Set the second array polynomials to zero
    for (uint32_t i = 0; i < numPoly; i++) {
        for (uint32_t j = 0; j < polyDepth; j++) {
            polynomials[1][i][j] = 0 + 0 * I;
        }
    }

    combinePoly(polynomials, order, denominator);

    // Free memory and return
    for (uint32_t i = 0; i < numArrays; i++) {
        for (uint32_t j = 0; j < numPoly; j++){
            free(polynomials[i][j]);
        }
        free(polynomials[i]);
    }
    free(polynomials);

    return;
}

// The double complex *ret array should have length: order + 1
static void combinePoly(double complex ***polynomials, uint16_t order, double complex *ret) {
    
    uint8_t  oddOrder  = order % 2;

    printf("Start calculating the polynomial\n");
    // Go through order/2 rounded up steps
    uint32_t steps = (order + oddOrder)/2;
    printf("number of steps: %d\n", steps);

    for (uint32_t step = 1; step <= steps; step++) {
        printf("\tperforming step %d\n", step);

        printf("\t\tNumber of combines: %d\n", steps/((step)));
        for (uint32_t combine = 0; combine < steps/((step)); combine++) {

            uint8_t arrFrom = !(step%2);
            uint8_t arrTo   = step%2;
            uint16_t polyIncrement = (uint16_t) pow(2, step);
            uint16_t polyX = polyIncrement * combine;
            uint16_t polyY = (uint16_t) combine * polyIncrement + pow(2, step - 1);

            printf("calculate if there was an uneven number of results last step\n");
            // Detect odd last thingies :)
            uint16_t isLastPolyOdd = order;
            printf("\torder: %d\n", isLastPolyOdd);
            for (uint16_t i = 1; i < step; i++) {
                isLastPolyOdd = (isLastPolyOdd + (isLastPolyOdd % 2))/2;
            }
            isLastPolyOdd %= 2;
            printf("\tisLastPolyOdd final: %d\n", isLastPolyOdd);


            if (isLastPolyOdd && (combine + 1 >= steps/((step)))) {
                double complex *temp;
                temp = polynomials[arrFrom][polyX];
                polynomials[arrFrom][polyX] = polynomials[arrTo][polyX];
                polynomials[arrTo][polyX] = temp;
            }
            else {
                polyMul(polynomials[arrFrom][polyX], polynomials[arrFrom][polyY], (uint16_t) pow(2, step - 1), polynomials[arrTo][polyX]);

                printf("Check for last step\n");
                if (step + 1 > steps) {
                    for (uint32_t i = 0; i <= order ; i++) {
                        ret[i] = polynomials[arrTo][polyX][i];
                    }
                }
            }

        }
    }

}

// The double complex *numerator array should have length order + 1
void digitalNum(double complex *poles, uint16_t order, double fSample, double complex *numerator) {

    printf("starting digitalDen\n");

    uint8_t  numArrays = 2;
    uint8_t  oddOrder  = order % 2;
    uint32_t numPoly   = order + oddOrder;
    uint32_t polyDepth = numPoly + 1;

    double samplePeriod = 1 / fSample;

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
        for (uint32_t j = 0; j < numPoly; j++) {
            polynomials[i][j] = malloc(polySizeAlloc);
            if (polynomials[i][j] == NULL) {
                printf("Not enough memory\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    
    printf("Filling the first order polynomials in the array's\n");
    // Fill the first order polynomials in array 0 Pi*T*z + Pi*T
    for (uint32_t i = 0; i < order; i++) {
        polynomials[0][i][0] = poles[i] * samplePeriod;
        polynomials[0][i][1] = poles[i] * samplePeriod;

        // Set the rest of the polynomial to 0+0j
        for (uint32_t j = 2; j < polySize; j++) {
            polynomials[0][i][j] = 0 + 0 * I;
        }

        // Set the second polynomial array completlly to 0
        for (uint32_t j = 0; j < polySize; j++) {
            polynomials[1][i][j] = 0 + 0 * I;
        }
    }
    printf("\n");

    // Set the second array polynomials to zero
    for (uint32_t i = 0; i < numPoly; i++) {
        for (uint32_t j = 0; j < polyDepth; j++) {
            polynomials[1][i][j] = 0 + 0 * I;
        }
    }

    combinePoly(polynomials, order, numerator);

    // Free memory and return
    for (uint32_t i = 0; i < numArrays; i++) {
        for (uint32_t j = 0; j < numPoly; j++){
            free(polynomials[i][j]);
        }
        free(polynomials[i]);
    }
    free(polynomials);

    return;
}

// The double complex *denominator array should have length order + 1
void digitalDen(double complex *poles, uint16_t order, double fSample, double complex *denominator) {

    printf("starting digitalDen\n");

    uint8_t  numArrays = 2;
    uint8_t  oddOrder  = order % 2;
    uint32_t numPoly   = order + oddOrder;
    uint32_t polyDepth = numPoly + 1;

    double samplePeriod = 1 / fSample;

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
        for (uint32_t j = 0; j < numPoly; j++) {
            polynomials[i][j] = malloc(polySizeAlloc);
            if (polynomials[i][j] == NULL) {
                printf("Not enough memory\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    
    printf("Filling the first order polynomials in the array's\n");
    // Fill the first order polynomials in array 0 ([2-Pi*T]z-(2+Pi*T))
    for (uint32_t i = 0; i < order; i++) {
        polynomials[0][i][0] = -1 * (2 + poles[i] * samplePeriod);
        polynomials[0][i][1] = 2 - poles[i] * samplePeriod;

        // Set the rest of the polynomial to 0+0j
        for (uint32_t j = 2; j < polySize; j++) {
            polynomials[0][i][j] = 0 + 0 * I;
        }

        // Set the second polynomial array completlly to 0
        for (uint32_t j = 0; j < polySize; j++) {
            polynomials[1][i][j] = 0 + 0 * I;
        }
    }
    printf("\n");

    // Set the second array polynomials to zero
    for (uint32_t i = 0; i < numPoly; i++) {
        for (uint32_t j = 0; j < polyDepth; j++) {
            polynomials[1][i][j] = 0 + 0 * I;
        }
    }

    combinePoly(polynomials, order, denominator);

    // Free memory and return
    for (uint32_t i = 0; i < numArrays; i++) {
        for (uint32_t j = 0; j < numPoly; j++){
            free(polynomials[i][j]);
        }
        free(polynomials[i]);
    }
    free(polynomials);

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