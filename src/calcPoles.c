#include <stdio.h>
#include <math.h>
#include <complex.h>

#include "calcPoles.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


void chebyshev(double cutOffFreq, uint16_t order, double ripple, double complex *poles) {
    double rimpleFactor = sqrt(pow(10, ripple/10)-1);
    double A  = asinh(1/rimpleFactor)/order;
    //double sinhA = sinh(A);
    //double kc = tanh(A);

    //printf("epsilon=%f\n", rimpleFactor);
    //printf("A = %f\n", A);
    //printf("pole1 = %f+%fj\n",
    //    sinh(A), cosh(A));

    for (uint32_t i = 0; i < order; i++) {
        poles[i] = cutOffFreq * (-1 * sinh(A) * sin(( M_PI * ( 2*(i+1) - 1) )/( 2 * order )) + I*cosh(A)*cos(( M_PI * ( 2*(i+1) - 1) )/( 2 * order )));
    }

    return;
}



